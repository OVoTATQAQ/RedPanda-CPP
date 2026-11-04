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
#include <QFontDatabase>
#include <QGlyphRun>
#include <QRawFont>
#include <QTextLayout>
#include "src/utils/font.h"

class TestInterfaceFont : public QObject {
    Q_OBJECT
private slots:
    void preservePreferenceAndPointSize() {
        const QString family = QFontDatabase::systemFont(QFontDatabase::GeneralFont).family();
        QFont font = makeInterfaceFont(family, 13, "zh_CN");
        QCOMPARE(font.families().first(), family);
        QCOMPARE(font.pointSize(), 13);
        QCOMPARE(font.pixelSize(), -1);
    }
    void missingFamilyUsesUiLanguage() {
        QFont font = makeInterfaceFont("RedPanda Missing Font 79A7", 11, "zh_CN");
        QCOMPARE(font.families().first(), defaultUiFont("zh_CN"));
        QVERIFY(!font.families().contains("RedPanda Missing Font 79A7"));
    }
    void commonChineseUsesConsistentFace() {
#if defined(Q_OS_WIN)
        QStringList chineseFamilies{"Microsoft YaHei UI", "Microsoft YaHei", "SimHei"};
        QString latinFamily = "Segoe UI";
#elif defined(Q_OS_MACOS)
        QStringList chineseFamilies{"PingFang SC"};
        QString latinFamily = "Helvetica Neue";
#else
        QStringList chineseFamilies{"Noto Sans CJK SC"};
        QString latinFamily = "DejaVu Sans";
#endif
        const QStringList installed = QFontDatabase().families();
        QString preferred;
        for (const QString &family : chineseFamilies) {
            if (installed.contains(family, Qt::CaseInsensitive)) {
                preferred = family;
                break;
            }
        }
        if (preferred.isEmpty() || !installed.contains(latinFamily, Qt::CaseInsensitive))
            QSKIP("Chinese and Latin test fonts are not installed on this runner.");
        QCOMPARE(defaultUiFont("zh_CN"), preferred);
        QFont font = makeInterfaceFont(latinFamily, 11, "zh_CN");
        QVERIFY(font.families().contains(preferred));
        const QString text = QString::fromUtf8(u8"\u6587\u4ef6\u540d\u751f\u6210\u4fdd\u5b58\u6a21\u578b\u5bc6\u94a5\u5b57\u4f53\u89c6\u56fe");
        QTextLayout layout(text, font);
        layout.beginLayout();
        auto line = layout.createLine();
        line.setLineWidth(1000);
        layout.endLayout();
        const auto runs = layout.glyphRuns();
        QVERIFY(!runs.isEmpty());
        const QString expected = QRawFont::fromFont(QFont(preferred)).familyName();
        for (const auto &run : runs) {
            QCOMPARE(run.rawFont().familyName(), expected);
            for (quint32 glyph : run.glyphIndexes())
                QVERIFY(glyph != 0);
        }
    }
};
QTEST_MAIN(TestInterfaceFont)
#include "test-interfacefont.moc"
