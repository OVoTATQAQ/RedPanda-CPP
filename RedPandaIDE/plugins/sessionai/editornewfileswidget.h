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
#ifndef EDITORNEWFILESWIDGET_H
#define EDITORNEWFILESWIDGET_H
#include "codenaming.h"
#include <QWidget>
#include <functional>
class QLineEdit;
class QComboBox;
class QCheckBox;
class QLabel;
class EditorNewFilesWidget : public QWidget {
    Q_OBJECT
public:
    EditorNewFilesWidget(CodeNaming::Preferences &preferences, const QString &settingsFile,
                         std::function<QString()> defaultSuffix, QWidget *parent = nullptr);
private:
    void apply();
    void updatePreview();
    CodeNaming::Preferences &mPreferences;
    QString mSettingsFile;
    std::function<QString()> mDefaultSuffix;
    QLineEdit *mRoot;
    QComboBox *mFolderRule, *mFilenameRule;
    QCheckBox *mDirectSave;
    QLabel *mPreview;
};
#endif
