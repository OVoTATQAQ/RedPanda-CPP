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
#include "fileworkflowloader.h"
#include <QAction>
#include <QDir>
#include <QSettings>
#include <QSignalBlocker>
#include <utility>

#ifdef REDPANDA_STATIC_FILE_PLUGIN
#include <QJsonObject>
Q_IMPORT_PLUGIN(SessionAiPlugin)
#else
#include "../settings/dirsettings.h"

static QString bundledPluginFile()
{
    const QString relative = "fileplugins/sessionai";
#if defined(Q_OS_MACOS) && FILESYSTEM_LAYOUT == FILESYSTEM_LAYOUT_bundle
    return QDir(DirSettings::appDir()).absoluteFilePath("../PlugIns/" + relative);
#else
    const QDir installed(DirSettings::appLibexecDir());
    if (installed.exists("fileplugins"))
        return installed.absoluteFilePath(relative);
    return QDir(DirSettings::appDir()).absoluteFilePath(relative);
#endif
}
#endif

static constexpr auto enabledKey = "Plugins/sessionAI";

FileWorkflowLoader::FileWorkflowLoader(FileWorkflowContext context, QObject *parent,
                                       const QString &pluginFile) :
    QObject(parent), mContext(std::move(context)), mPluginFile(pluginFile),
    mToggle(new QAction(tr("Session folders and AI filenames"), this))
{
    mToggle->setObjectName("actionSessionAiPlugin");
    mToggle->setCheckable(true);
    connect(mToggle, &QAction::toggled, this, [this](bool enabled) {
        if (enabled && !start(true)) {
            QSignalBlocker blocker(mToggle);
            mToggle->setChecked(false);
            return;
        }
        if (!enabled)
            stop();
        QSettings(mContext.settingsFile, QSettings::IniFormat).setValue(enabledKey, enabled);
    });
}

FileWorkflowLoader::~FileWorkflowLoader()
{
    stop();
}

void FileWorkflowLoader::restore()
{
    QSettings settings(mContext.settingsFile, QSettings::IniFormat);
    if (!settings.value(enabledKey, false).toBool())
        return;
    const bool enabled = start(false);
    if (!enabled)
        settings.setValue(enabledKey, false);
    QSignalBlocker blocker(mToggle);
    mToggle->setChecked(enabled);
}

bool FileWorkflowLoader::start(bool showPanel)
{
    if (mActive)
        return true;
#ifdef REDPANDA_STATIC_FILE_PLUGIN
    for (const QStaticPlugin &plugin : QPluginLoader::staticPlugins()) {
        if (plugin.metaData().value("IID").toString() == FileWorkflow_iid) {
            mActive = qobject_cast<FileWorkflow *>(plugin.instance());
            break;
        }
    }
#else
    mLoader.setFileName(mPluginFile.isEmpty() ? bundledPluginFile() : mPluginFile);
    mActive = qobject_cast<FileWorkflow *>(mLoader.instance());
#endif
    if (!mActive) {
        QString error;
#ifndef REDPANDA_STATIC_FILE_PLUGIN
        error = mLoader.errorString();
        if (mLoader.isLoaded())
            mLoader.unload();
#endif
        emit loadFailed(tr("Could not load the file workflow plugin. %1").arg(error).trimmed());
        return false;
    }
    mActive->activate(mContext, showPanel);
    emit activeChanged(mActive);
    return true;
}

void FileWorkflowLoader::stop()
{
    if (!mActive)
        return;
    FileWorkflow *plugin = mActive;
    mActive = nullptr;
    emit activeChanged(nullptr);
    plugin->deactivate();
#ifndef REDPANDA_STATIC_FILE_PLUGIN
    mLoader.unload();
#endif
}
