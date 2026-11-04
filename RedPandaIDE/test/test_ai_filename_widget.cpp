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
#include "test_ai_filename_widget.h"
#include "src/widgets/aifilenamewidget.h"
#include <QtTest>
#include <QComboBox>
#include <QLineEdit>
#include <QSettings>
#include <QTemporaryDir>

void TestAiFilenameWidget::switchProviders()
{
    QTemporaryDir root;
    AiFilenameWidget panel([]() -> Editor * { return nullptr; }, root.filePath("settings.ini"));
    auto provider = panel.findChild<QComboBox *>("aiProvider");
    auto model = panel.findChild<QComboBox *>("aiModel");
    auto endpoint = panel.findChild<QLineEdit *>("aiEndpoint");
    auto key = panel.findChild<QLineEdit *>("aiApiKey");
    QVERIFY(provider && model && endpoint && key);
    QCOMPARE(provider->currentData().toString(), QString("openai"));
    key->setText("test-key-not-a-credential");
    provider->setCurrentIndex(provider->findData("deepseek"));
    QCOMPARE(endpoint->text(), QString("https://api.deepseek.com/chat/completions"));
    QCOMPARE(model->currentText(), QString("deepseek-chat"));
    QVERIFY(model->findText("deepseek-reasoner") >= 0);
    QVERIFY(key->text().isEmpty());
    QVERIFY(model->isEditable());
    model->setCurrentText("provider-custom-model");
    provider->setCurrentIndex(provider->findData("custom"));
    endpoint->setText("https://example.com/v1/chat/completions");
    model->setCurrentText("custom-model-id");
    provider->setCurrentIndex(provider->findData("deepseek"));
    QCOMPARE(model->currentText(), QString("provider-custom-model"));
    provider->setCurrentIndex(provider->findData("custom"));
    QCOMPARE(endpoint->text(), QString("https://example.com/v1/chat/completions"));
    QCOMPARE(model->currentText(), QString("custom-model-id"));
}

void TestAiFilenameWidget::restoreLegacyDeepSeekSettings()
{
    QTemporaryDir root;
    const QString file = root.filePath("settings.ini");
    {
        QSettings settings(file, QSettings::IniFormat);
        settings.setValue("AIFileName/endpoint", "https://api.deepseek.com/v1/chat/completions");
        settings.setValue("AIFileName/model", "deepseek-reasoner");
    }
    AiFilenameWidget panel([]() -> Editor * { return nullptr; }, file);
    QCOMPARE(panel.findChild<QComboBox *>("aiProvider")->currentData().toString(), QString("deepseek"));
    QCOMPARE(panel.findChild<QComboBox *>("aiModel")->currentText(), QString("deepseek-reasoner"));
    QCOMPARE(panel.findChild<QLineEdit *>("aiEndpoint")->text(),
             QString("https://api.deepseek.com/v1/chat/completions"));
}

void TestAiFilenameWidget::restoreSavedProviderProfile()
{
    QTemporaryDir root;
    const QString file = root.filePath("settings.ini");
    {
        QSettings settings(file, QSettings::IniFormat);
        settings.setValue("AIFileName/provider", "deepseek");
        settings.setValue("AIFileName/providers/deepseek/endpoint", "https://api.deepseek.com/chat/completions");
        settings.setValue("AIFileName/providers/deepseek/model", "deepseek-chat");
    }
    AiFilenameWidget panel([]() -> Editor * { return nullptr; }, file);
    QCOMPARE(panel.findChild<QComboBox *>("aiProvider")->currentData().toString(), QString("deepseek"));
    QCOMPARE(panel.findChild<QComboBox *>("aiModel")->currentText(), QString("deepseek-chat"));
    QVERIFY(panel.findChild<QLineEdit *>("aiApiKey")->text().isEmpty());
}
