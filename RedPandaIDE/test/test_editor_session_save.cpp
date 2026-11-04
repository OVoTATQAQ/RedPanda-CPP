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
#include "test_editor_session_save.h"
#include "src/editor.h"
#include <QtTest>
#include <QTemporaryDir>
#include <QFile>
#include <QFileDialog>
#include <QTimer>
#include <QScopeGuard>
#include "src/systemconsts.h"
#include "src/settings/editorsettings.h"

void TestEditorSessionSave::saveNewAndUpdate()
{
    init_editor();
    QTemporaryDir root;
    QString name = root.filePath("untitled1.cpp");
    mEditor->setFilename(name);
    mEditor->setProperty("sessionCodeFile", true);
    mEditor->setGetOpennedFunc([this](const QString &name) -> Editor * {
        return name == mEditor->filename() ? mEditor.get() : nullptr;
    });
    mEditor->loadContent(QString("int main() { return 0; }"));
    QVERIFY(mEditor->save(false, false));
    QVERIFY(!mEditor->isNew());
    QCOMPARE(mEditor->filename(), name);
    mEditor->loadContent(QString("int main() { return 1; }"));
    QVERIFY(mEditor->save(false, false));
    QFile result(name);
    QVERIFY(result.open(QIODevice::ReadOnly));
    QVERIFY(result.readAll().contains("return 1"));
    QVERIFY(!QFile::exists(root.filePath("untitled1_1.cpp")));
}

void TestEditorSessionSave::collisionKeepsOriginal()
{
    init_editor();
    QTemporaryDir root;
    QString name = root.filePath("untitled1.cpp");
    QFile original(name);
    QVERIFY(original.open(QIODevice::WriteOnly));
    original.write("previous code");
    original.close();
    mEditor->setFilename(name);
    mEditor->setProperty("sessionCodeFile", true);
    mEditor->loadContent(QString("int main() {}"));
    QVERIFY(mEditor->save(false, false));
    QCOMPARE(mEditor->filename(), root.filePath("untitled1_1.cpp"));
    QVERIFY(original.open(QIODevice::ReadOnly));
    QCOMPARE(original.readAll(), QByteArray("previous code"));
}

void TestEditorSessionSave::settingsRoundTrip()
{
    QTemporaryDir root;
    const QString config = root.filePath("settings.ini");
    {
        SettingsPersistor persistor(config);
        EditorSettings settings(&persistor);
        settings.load();
        QCOMPARE(settings.newFileSaveRoot(), CodeNaming::defaultSaveRoot());
        QCOMPARE(settings.newFileFolderRule(), CodeNaming::FolderRule::PerSession);
        QCOMPARE(settings.newFileNameRule(), CodeNaming::FilenameRule::Numbered);
        QVERIFY(settings.newFileAutoSave());
        settings.setNewFileSaveRoot(root.filePath("code"));
        settings.setNewFileFolderRule(CodeNaming::FolderRule::PerDay);
        settings.setNewFileNameRule(CodeNaming::FilenameRule::Timestamp);
        settings.setNewFileAutoSave(false);
        settings.save();
        QCOMPARE(persistor.sync(), QSettings::NoError);
    }
    SettingsPersistor persistor(config);
    EditorSettings restored(&persistor);
    restored.load();
    QCOMPARE(restored.newFileSaveRoot(), root.filePath("code"));
    QCOMPARE(restored.newFileFolderRule(), CodeNaming::FolderRule::PerDay);
    QCOMPARE(restored.newFileNameRule(), CodeNaming::FilenameRule::Timestamp);
    QVERIFY(!restored.newFileAutoSave());
}

void TestEditorSessionSave::invalidRulesUseDefaults()
{
    QTemporaryDir root;
    const QString config = root.filePath("settings.ini");
    {
        QSettings settings(config, QSettings::IniFormat);
        settings.setValue("Editor/new_file_save_root", "relative/path");
        settings.setValue("Editor/new_file_folder_rule", 99);
        settings.setValue("Editor/new_file_name_rule", -1);
    }
    SettingsPersistor persistor(config);
    EditorSettings restored(&persistor);
    restored.load();
    QCOMPARE(restored.newFileSaveRoot(), CodeNaming::defaultSaveRoot());
    QCOMPARE(restored.newFileFolderRule(), CodeNaming::FolderRule::PerSession);
    QCOMPARE(restored.newFileNameRule(), CodeNaming::FilenameRule::Numbered);
}

void TestEditorSessionSave::manualFirstSaveCanBeCancelled()
{
    SystemConsts systemConsts;
    auto originalConsts = pSystemConsts;
    pSystemConsts = &systemConsts;
    auto restoreConsts = qScopeGuard([originalConsts]() { pSystemConsts = originalConsts; });
    init_editor();
    QTemporaryDir root;
    QString name = root.filePath("untitled1.cpp");
    mEditor->setFilename(name);
    mEditor->setProperty("sessionCodeFile", false);
    mEditor->loadContent(QString("int main() {}"));
    QApplication::setAttribute(Qt::AA_DontUseNativeDialogs);
    bool dialogShown = false;
    QTimer::singleShot(0, [&dialogShown]() {
        for (QWidget *widget : QApplication::topLevelWidgets()) {
            if (auto dialog = qobject_cast<QFileDialog *>(widget)) {
                dialogShown = true;
                dialog->reject();
            }
        }
    });
    QVERIFY(!mEditor->save(false, false));
    QVERIFY(dialogShown);
    QVERIFY(mEditor->isNew());
    QCOMPARE(mEditor->filename(), name);
    QVERIFY(!QFile::exists(name));
    QVERIFY(mEditor->text().contains("int main()"));
}
