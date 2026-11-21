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
#include "fileworkflowfixture.h"
#include "src/systemconsts.h"
#include <QtTest>
#include <QCheckBox>
#include <QComboBox>
#include <QDockWidget>
#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QScopeGuard>
#include <QTimer>

static void useFixedFolder(const QTemporaryDir &root, bool direct = true)
{
    QSettings settings(root.filePath("settings.ini"), QSettings::IniFormat);
    settings.setValue("Editor/new_file_save_root", root.path());
    settings.setValue("Editor/new_file_folder_rule", 2);
    settings.setValue("Editor/new_file_auto_save", direct);
}

void TestEditorSessionSave::saveNewAndUpdate()
{
    init_editor();
    QTemporaryDir root;
    useFixedFolder(root);
    FileWorkflowFixture host(root, mEditor.get());
    host.enable();
    QVERIFY(host.loader.active());
    host.newFile(mEditor.get());
    QString name = root.filePath("untitled1.cpp");
    QCOMPARE(mEditor->suggestedSaveName(), name);
    mEditor->loadContent(QString("int main() { return 0; }"));
    QVERIFY(mEditor->save(false, false));
    QVERIFY(!mEditor->isNew());
    QCOMPARE(mEditor->filename(), name);
    host.disable();
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
    useFixedFolder(root);
    FileWorkflowFixture host(root, mEditor.get());
    host.enable();
    QVERIFY(host.loader.active());
    host.newFile(mEditor.get());
    QFile original(root.filePath("untitled1.cpp"));
    QVERIFY(original.open(QIODevice::WriteOnly));
    original.write("previous code");
    original.close();
    mEditor->loadContent(QString("int main() {}"));
    QVERIFY(mEditor->save(false, false));
    QCOMPARE(mEditor->filename(), root.filePath("untitled1_1.cpp"));
    QVERIFY(original.open(QIODevice::ReadOnly));
    QCOMPARE(original.readAll(), QByteArray("previous code"));
}

void TestEditorSessionSave::settingsRoundTrip()
{
    init_editor();
    QTemporaryDir root;
    {
        FileWorkflowFixture host(root, mEditor.get());
        host.enable();
        QVERIFY(host.loader.active());
        host.window.findChild<QLineEdit *>("newFileSaveRoot")->setText(root.filePath("daily"));
        host.window.findChild<QComboBox *>("newFileFolderRule")->setCurrentIndex(1);
        host.window.findChild<QComboBox *>("newFileNameRule")->setCurrentIndex(1);
        host.window.findChild<QCheckBox *>("newFileAutoSave")->setChecked(false);
        host.window.findChild<QPushButton *>("applyNewFilePreferences")->click();
    }
    FileWorkflowFixture restored(root, mEditor.get());
    QVERIFY(restored.loader.active());
    QVERIFY(restored.loader.toggleAction()->isChecked());
    restored.newFile(mEditor.get(), 41);
    QFileInfo example(mEditor->suggestedSaveName());
    QCOMPARE(example.absolutePath(), root.filePath("daily/" + QDate::currentDate().toString("yyyy-MM-dd")));
    QVERIFY(example.fileName().startsWith("code_"));
    QVERIFY(example.fileName().endsWith("_41.cpp"));
    QVERIFY(!restored.window.findChild<QCheckBox *>("newFileAutoSave")->isChecked());
}

void TestEditorSessionSave::invalidRulesUseDefaults()
{
    QTemporaryDir root;
    QSettings settings(root.filePath("settings.ini"), QSettings::IniFormat);
    settings.setValue("Editor/new_file_save_root", root.path());
    settings.setValue("Editor/new_file_folder_rule", 99);
    settings.setValue("Editor/new_file_name_rule", -1);
    FileWorkflowFixture host(root);
    host.enable();
    QVERIFY(host.loader.active());
    QCOMPARE(host.window.findChild<QComboBox *>("newFileFolderRule")->currentIndex(), 0);
    QCOMPARE(host.window.findChild<QComboBox *>("newFileNameRule")->currentIndex(), 0);
    QVERIFY(host.window.findChild<QCheckBox *>("newFileAutoSave")->isChecked());
}

void TestEditorSessionSave::manualFirstSaveCanBeCancelled_data()
{
    QTest::addColumn<bool>("direct");
    QTest::addColumn<bool>("disableBeforeSave");
    QTest::newRow("manual first save") << false << false;
    QTest::newRow("disabled after creating a direct-save file") << true << true;
}

void TestEditorSessionSave::manualFirstSaveCanBeCancelled()
{
    QFETCH(bool, direct);
    QFETCH(bool, disableBeforeSave);
    SystemConsts systemConsts;
    auto originalConsts = pSystemConsts;
    pSystemConsts = &systemConsts;
    auto restoreConsts = qScopeGuard([originalConsts]() { pSystemConsts = originalConsts; });
    init_editor();
    QTemporaryDir root;
    useFixedFolder(root, direct);
    FileWorkflowFixture host(root, mEditor.get());
    host.enable();
    QVERIFY(host.loader.active());
    host.newFile(mEditor.get());
    mEditor->loadContent(QString("int main() {}"));
    if (disableBeforeSave)
        host.disable();
    const QString suggestion = disableBeforeSave ? QString("untitled1.cpp") : root.filePath("untitled1.cpp");
    QCOMPARE(mEditor->suggestedSaveName(), suggestion);
    QApplication::setAttribute(Qt::AA_DontUseNativeDialogs);
    QString shownPath;
    QTimer::singleShot(0, [&shownPath]() {
        if (auto dialog = qobject_cast<QFileDialog *>(QApplication::activeModalWidget())) {
            shownPath = dialog->selectedFiles().value(0);
            dialog->reject();
        }
    });
    QVERIFY(!mEditor->save(false, false));
    QCOMPARE(shownPath, QFileInfo(suggestion).absoluteFilePath());
    QVERIFY(mEditor->isNew());
    QVERIFY(!QFile::exists(root.filePath("untitled1.cpp")));
    QVERIFY(mEditor->text().contains("int main()"));
}

void TestEditorSessionSave::toggleLifecycle()
{
    init_editor();
    QTemporaryDir root;
    const QString codeRoot = root.filePath("code");
    {
        FileWorkflowFixture host(root, mEditor.get());
        QVERIFY(!host.loader.active());
        QVERIFY(!host.loader.toggleAction()->isChecked());
        QVERIFY(host.viewMenu.actions().isEmpty());
        host.newFile(mEditor.get());
        QVERIFY(!QFileInfo::exists(codeRoot));
        host.enable();
        QVERIFY(host.loader.active());
        QCOMPARE(mEditor->suggestedSaveName(), mEditor->filename());
        host.newFile(mEditor.get(), 2);
        const QString firstFolder = QFileInfo(mEditor->suggestedSaveName()).absolutePath();
        QVERIFY(QDir(codeRoot).exists());
        QPointer<QDockWidget> panel = host.window.findChild<QDockWidget *>("AIFileNameDock");
        QVERIFY(panel);
        QCOMPARE(host.viewMenu.actions().size(), 1);
        host.disable();
        QVERIFY(panel.isNull());
        QVERIFY(host.viewMenu.actions().isEmpty());
        QCOMPARE(mEditor->suggestedSaveName(), QString("untitled2.cpp"));
        host.enable();
        QVERIFY(host.loader.active());
        QCOMPARE(mEditor->suggestedSaveName(), QString("untitled2.cpp"));
        host.newFile(mEditor.get(), 3);
        QCOMPARE(QFileInfo(mEditor->suggestedSaveName()).absolutePath(), firstFolder);
        QCOMPARE(host.viewMenu.actions().size(), 1);
        host.disable();
    }
    FileWorkflowFixture restored(root);
    QVERIFY(!restored.loader.active());
    QVERIFY(restored.viewMenu.actions().isEmpty());
    QCOMPARE(QDir(codeRoot).entryList(QDir::Dirs | QDir::NoDotAndDotDot).size(), 1);
}

#ifndef REDPANDA_STATIC_FILE_PLUGIN
void TestEditorSessionSave::missingPluginRestoresStock()
{
    QTemporaryDir root;
    const QString config = root.filePath("settings.ini");
    QSettings settings(config, QSettings::IniFormat);
    settings.setValue("Plugins/sessionAI", true);
    QMainWindow window;
    QMenu viewMenu;
    FileWorkflowLoader loader({&window, &viewMenu, config, {}, {}, {}}, nullptr, root.filePath("missing"));
    QSignalSpy errors(&loader, &FileWorkflowLoader::loadFailed);
    loader.restore();
    QCOMPARE(errors.count(), 1);
    QVERIFY(!loader.active());
    QVERIFY(!loader.toggleAction()->isChecked());
    QVERIFY(viewMenu.actions().isEmpty());
    QVERIFY(!settings.value("Plugins/sessionAI").toBool());
}
#endif
