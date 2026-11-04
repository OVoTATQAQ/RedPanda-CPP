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
#include <QtTest>
#include <QTemporaryDir>
#include <QFile>
#include <QDir>
#include "src/utils/codenaming.h"

class TestCodeNaming : public QObject {
    Q_OBJECT
private slots:
    void distinctSessions() {
        QTemporaryDir root;
        QVERIFY(root.isValid());
        QString a = CodeNaming::createSessionDirectory(root.path());
        QString b = CodeNaming::createSessionDirectory(root.path());
        QVERIFY(!a.isEmpty());
        QVERIFY(a != b);
        QVERIFY(QDir(a).exists());
        QVERIFY(QDir(b).exists());
    }
    void occupiedRoot() {
        QTemporaryDir root;
        QFile file(root.filePath("file"));
        QVERIFY(file.open(QIODevice::WriteOnly));
        file.close();
        QString error;
        QVERIFY(CodeNaming::createSessionDirectory(file.fileName(), &error).isEmpty());
        QVERIFY(!error.isEmpty());
    }
    void reserveWithoutOverwrite() {
        QTemporaryDir root;
        QString requested = root.filePath("binary_search.cpp");
        QFile original(requested);
        QVERIFY(original.open(QIODevice::WriteOnly));
        original.write("old code");
        original.close();
        QString second = CodeNaming::reserveFile(requested);
        QString third = CodeNaming::reserveFile(requested);
        QCOMPARE(second, root.filePath("binary_search_1.cpp"));
        QCOMPARE(third, root.filePath("binary_search_2.cpp"));
        QVERIFY(original.open(QIODevice::ReadOnly));
        QCOMPARE(original.readAll(), QByteArray("old code"));
        QVERIFY(CodeNaming::reserveFile(root.filePath("missing/file.cpp")).isEmpty());
    }
    void untrustedNames() {
        QCOMPARE(CodeNaming::safeStem("../../a/b: c?*"), QString("a_b_c"));
        QCOMPARE(CodeNaming::safeStem("CON"), QString("code_CON"));
        QCOMPARE(CodeNaming::safeStem("lpt1"), QString("code_lpt1"));
        QCOMPARE(CodeNaming::safeStem("..."), QString());
        QCOMPARE(CodeNaming::safeStem(QString(200, 'a')).size(), 80);
        QCOMPARE(CodeNaming::safeStem("binary_search"), QString("binary_search"));
    }
};
QTEST_GUILESS_MAIN(TestCodeNaming)
#include "test-codenaming.moc"
