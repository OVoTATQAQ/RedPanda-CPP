/*
 * Copyright (C) 2020-2022 Roy Qu (royqh1979@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "sessionaiplugin.h"
#include "aifilenamewidget.h"
#include "editornewfileswidget.h"
#include <QApplication>
#include <QDir>
#include <QDockWidget>
#include <QFile>
#include <QFileInfo>
#include <QMainWindow>
#include <QMenu>
#include <QSettings>
#include <QLocale>
#include <QMessageBox>
#include <QTabWidget>
#include <QVariant>

void SessionAiPlugin::activate(const FileWorkflowContext &context, bool showPanel)
{
#ifdef QT_STATICPLUGIN
    Q_INIT_RESOURCE(sessionai_qmake_qmake_qm_files);
#endif
    QSettings settings(context.settingsFile, QSettings::IniFormat);
    const QString language = settings.value("Environment/language", QLocale::system().name()).toString();
    if (mTranslator.load("sessionai_" + language, ":/i18n/"))
        qApp->installTranslator(&mTranslator);
    mPreferences = CodeNaming::Preferences::load(context.settingsFile);
    mDock = new QDockWidget(tr("Session folders and AI filenames"), context.window);
    mDock->setObjectName("AIFileNameDock");
    auto tabs = new QTabWidget(mDock);
    tabs->addTab(new EditorNewFilesWidget(mPreferences, context.settingsFile, context.defaultSuffix, tabs),
                 tr("New File Saving"));
    tabs->addTab(new AiFilenameWidget(context, tabs), tr("AI filename"));
    mDock->setWidget(tabs);
    if (!context.window->restoreDockWidget(mDock)) {
        context.window->addDockWidget(Qt::RightDockWidgetArea, mDock);
        mDock->hide();
    }
    context.viewMenu->addAction(mDock->toggleViewAction());
    if (showPanel)
        mDock->show();
    saveDirectory();
}

void SessionAiPlugin::deactivate()
{
    // The dock owns both pages, the view-menu action and the network manager.
    delete mDock;
    mDock = nullptr;
    mPendingFiles.clear();
    mDirectoryError.clear();
    qApp->removeTranslator(&mTranslator);
#ifdef QT_STATICPLUGIN
    Q_CLEANUP_RESOURCE(sessionai_qmake_qmake_qm_files);
#endif
}

QString SessionAiPlugin::saveDirectory()
{
    QString error;
    QString directory;
    if (mPreferences.folderRule == CodeNaming::FolderRule::PerSession) {
        // A launch keeps its folder even if the user unloads and reloads the plugin.
        static constexpr auto key = "sessionAiLaunchDirectories";
        QVariantMap folders = qApp->property(key).toMap();
        directory = folders.value(mPreferences.root).toString();
        if (directory.isEmpty()) {
            directory = CodeNaming::createSessionDirectory(mPreferences.root, &error);
            if (!directory.isEmpty()) {
                folders.insert(mPreferences.root, directory);
                qApp->setProperty(key, folders);
            }
        } else if (!QDir().mkpath(directory)) {
            error = directory;
            directory.clear();
        }
    } else {
        directory = CodeNaming::createSaveDirectory(mPreferences.root, mPreferences.folderRule, &error);
    }
    if (!error.isEmpty() && error != mDirectoryError)
        QMessageBox::warning(mDock, tr("Code folder"),
                             tr("Cannot create the code folder in %1. New files will use Save As.").arg(error));
    mDirectoryError = error;
    return directory;
}

void SessionAiPlugin::newFile(QObject *document, const QString &filename, int number)
{
    const QString directory = saveDirectory();
    if (directory.isEmpty())
        return;
    QString stem = CodeNaming::defaultFileStem(mPreferences.filenameRule, number);
    const QString suffix = QFileInfo(filename).suffix();
    if (!suffix.isEmpty())
        stem += "." + suffix;
    mPendingFiles.insert(document, {QDir(directory).filePath(stem), mPreferences.directSave});
    connect(document, &QObject::destroyed, mDock, [this](QObject *doc) {
        mPendingFiles.remove(doc);
    });
}

QString SessionAiPlugin::suggestedPath(const QObject *document) const
{
    return mPendingFiles.value(document).path;
}

std::optional<bool> SessionAiPlugin::firstSave(const QObject *document, bool force,
        const std::function<bool(const QString &)> &saveAs)
{
    auto it = mPendingFiles.constFind(document);
    if (it == mPendingFiles.constEnd() || (!it->direct && !force))
        return std::nullopt;

    QString error;
    const QString destination = CodeNaming::reserveFile(it->path, &error);
    if (destination.isEmpty()) {
        QMessageBox::warning(mDock, tr("Save Error"), error);
        return false;
    }
    if (saveAs(destination))
        return true;
    QFile::remove(destination);
    return false;
}
