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
#ifndef AIFILENAMEWIDGET_H
#define AIFILENAMEWIDGET_H
#include <QWidget>
#include <QPointer>
#include <QHash>
#include <QPair>
#include <functional>
class Editor;
class QLabel;
class QComboBox;
class QLineEdit;
class QPushButton;
class QNetworkAccessManager;
class AiFilenameWidget : public QWidget {
    Q_OBJECT
public:
    AiFilenameWidget(std::function<Editor *()> currentEditor,
                     const QString &settingsFile, QWidget *parent = nullptr);
private:
    void changeProvider();
    void generate();
    void saveSuggested();
    std::function<Editor *()> mCurrentEditor;
    QString mSettingsFile;
    QLineEdit *mEndpoint, *mKey, *mName;
    QComboBox *mProvider, *mModel;
    QString mCurrentProvider;
    QHash<QString, QPair<QString, QString>> mProviderConfigs;
    QLabel *mStatus;
    QPushButton *mGenerate, *mSave;
    QNetworkAccessManager *mNetwork;
    QPointer<Editor> mSource;
    QString mSourceText, mSourceFilename;
};
#endif
