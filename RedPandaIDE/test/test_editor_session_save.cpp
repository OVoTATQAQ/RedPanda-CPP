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
