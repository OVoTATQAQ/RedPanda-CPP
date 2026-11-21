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
#include "fileworkflowfixture.h"
#include <QPushButton>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtTest>
#include <QComboBox>
#include <QLineEdit>
#include <QSettings>
#include <QTemporaryDir>

void TestAiFilenameWidget::switchProviders()
{
    QTemporaryDir root;
    FileWorkflowFixture host(root);
    host.enable();
    QVERIFY(host.loader.active());
    auto provider = host.window.findChild<QComboBox *>("aiProvider");
    auto model = host.window.findChild<QComboBox *>("aiModel");
    auto endpoint = host.window.findChild<QLineEdit *>("aiEndpoint");
    auto key = host.window.findChild<QLineEdit *>("aiApiKey");
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
    FileWorkflowFixture host(root);
    host.enable();
    QVERIFY(host.loader.active());
    QCOMPARE(host.window.findChild<QComboBox *>("aiProvider")->currentData().toString(), QString("deepseek"));
    QCOMPARE(host.window.findChild<QComboBox *>("aiModel")->currentText(), QString("deepseek-reasoner"));
    QCOMPARE(host.window.findChild<QLineEdit *>("aiEndpoint")->text(),
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
    FileWorkflowFixture host(root);
    host.enable();
    QVERIFY(host.loader.active());
    QCOMPARE(host.window.findChild<QComboBox *>("aiProvider")->currentData().toString(), QString("deepseek"));
    QCOMPARE(host.window.findChild<QComboBox *>("aiModel")->currentText(), QString("deepseek-chat"));
    QVERIFY(host.window.findChild<QLineEdit *>("aiApiKey")->text().isEmpty());
}

void TestAiFilenameWidget::disableWithPendingRequest()
{
    QTemporaryDir root;
    init_editor();
    Editor &editor = *mEditor;
    editor.setFilename("untitled1.cpp");
    editor.loadContent(QString("int x;"));
    QTcpServer server;
    QVERIFY(server.listen(QHostAddress::LocalHost));
    FileWorkflowFixture host(root, &editor);
    host.enable();
    QVERIFY(host.loader.active());
    host.window.findChild<QLineEdit *>("aiEndpoint")->setText(
            QString("https://127.0.0.1:%1/chat/completions").arg(server.serverPort()));
    host.window.findChild<QLineEdit *>("aiApiKey")->setText("local-test");
    auto generate = host.window.findChild<QPushButton *>("aiGenerate");
    generate->click();
    QVERIFY(!generate->isEnabled());
    QTRY_VERIFY(server.hasPendingConnections());
    auto socket = server.nextPendingConnection();
    host.disable();
    QTRY_COMPARE(socket->state(), QAbstractSocket::UnconnectedState);
    host.enable();
    QVERIFY(host.window.findChild<QLineEdit *>("aiApiKey")->text().isEmpty());
    QVERIFY(host.window.findChild<QPushButton *>("aiGenerate")->isEnabled());
}
