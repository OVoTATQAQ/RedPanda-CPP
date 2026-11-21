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
#include "editornewfileswidget.h"
#include <QMessageBox>
#include <QCheckBox>
#include <QComboBox>
#include <QDate>
#include <QDir>
#include <QFileDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <utility>

EditorNewFilesWidget::EditorNewFilesWidget(CodeNaming::Preferences &preferences, const QString &settingsFile,
                                         std::function<QString()> defaultSuffix, QWidget *parent) :
    QWidget(parent), mPreferences(preferences), mSettingsFile(settingsFile),
    mDefaultSuffix(std::move(defaultSuffix))
{
    auto layout = new QVBoxLayout(this);
    auto form = new QFormLayout;
    auto folder = new QWidget(this);
    auto folderLayout = new QHBoxLayout(folder);
    folderLayout->setContentsMargins(0, 0, 0, 0);
    mRoot = new QLineEdit(folder);
    mRoot->setObjectName("newFileSaveRoot");
    mRoot->setPlaceholderText(QDir::toNativeSeparators(CodeNaming::defaultSaveRoot()));
    auto browse = new QPushButton(tr("Browse..."), folder);
    folderLayout->addWidget(mRoot, 1);
    folderLayout->addWidget(browse);
    mFolderRule = new QComboBox(this);
    mFolderRule->setObjectName("newFileFolderRule");
    mFolderRule->addItem(tr("A separate folder for each launch"), int(CodeNaming::FolderRule::PerSession));
    mFolderRule->addItem(tr("One folder per day (yyyy-MM-dd)"), int(CodeNaming::FolderRule::PerDay));
    mFolderRule->addItem(tr("Use the selected folder directly"), int(CodeNaming::FolderRule::Fixed));
    mFilenameRule = new QComboBox(this);
    mFilenameRule->setObjectName("newFileNameRule");
    mFilenameRule->addItem(tr("Numbered names (untitled1.cpp)"), int(CodeNaming::FilenameRule::Numbered));
    mFilenameRule->addItem(tr("Timestamp names (code_date_time.cpp)"), int(CodeNaming::FilenameRule::Timestamp));
    mDirectSave = new QCheckBox(tr("Save new files directly using the default name on first Save"), this);
    mDirectSave->setObjectName("newFileAutoSave");
    form->addRow(tr("Save folder:"), folder);
    form->addRow(tr("Subfolder rule:"), mFolderRule);
    form->addRow(tr("Filename rule:"), mFilenameRule);
    layout->addLayout(form);
    layout->addWidget(mDirectSave);
    auto help = new QLabel(tr("When unchecked, the first Save opens Save As in the chosen folder. Occupied names receive a numeric suffix when saving directly. Settings apply to standalone files created afterwards; existing files keep their paths."), this);
    help->setWordWrap(true);
    layout->addWidget(help);
    mPreview = new QLabel(this);
    mPreview->setTextFormat(Qt::PlainText);
    mPreview->setWordWrap(true);
    mPreview->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout->addWidget(mPreview);
    auto applyButton = new QPushButton(tr("Apply"), this);
    applyButton->setObjectName("applyNewFilePreferences");
    layout->addWidget(applyButton, 0, Qt::AlignLeft);
    layout->addStretch();
    connect(applyButton, &QPushButton::clicked, this, &EditorNewFilesWidget::apply);
    connect(browse, &QPushButton::clicked, this, [this]() {
        QString folder = QFileDialog::getExistingDirectory(this, tr("Select the code save folder"), mRoot->text());
        if (!folder.isEmpty()) mRoot->setText(QDir::toNativeSeparators(folder));
    });
    connect(mRoot, &QLineEdit::textChanged, this, [this]() { updatePreview(); });
    connect(mFolderRule, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this]() { updatePreview(); });
    connect(mFilenameRule, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this]() { updatePreview(); });

    mRoot->setText(QDir::toNativeSeparators(mPreferences.root));
    mFolderRule->setCurrentIndex(mFolderRule->findData(int(mPreferences.folderRule)));
    mFilenameRule->setCurrentIndex(mFilenameRule->findData(int(mPreferences.filenameRule)));
    mDirectSave->setChecked(mPreferences.directSave);
    updatePreview();
}

void EditorNewFilesWidget::apply()
{
    QString root = QDir::fromNativeSeparators(mRoot->text().trimmed());
    if (root.isEmpty())
        root = CodeNaming::defaultSaveRoot();
    if (!QDir::isAbsolutePath(root)) {
        QMessageBox::warning(this, tr("Save folder"), tr("Please choose an absolute save folder path."));
        mRoot->setFocus();
        return;
    }
    mPreferences.root = QDir::cleanPath(root);
    mPreferences.folderRule = static_cast<CodeNaming::FolderRule>(mFolderRule->currentData().toInt());
    mPreferences.filenameRule = static_cast<CodeNaming::FilenameRule>(mFilenameRule->currentData().toInt());
    mPreferences.directSave = mDirectSave->isChecked();
    mPreferences.save(mSettingsFile);
    mRoot->setText(QDir::toNativeSeparators(mPreferences.root));
}

void EditorNewFilesWidget::updatePreview()
{
    QString root = QDir::fromNativeSeparators(mRoot->text().trimmed());
    if (root.isEmpty()) root = CodeNaming::defaultSaveRoot();
    switch (static_cast<CodeNaming::FolderRule>(mFolderRule->currentData().toInt())) {
    case CodeNaming::FolderRule::PerSession:
        root = QDir(root).filePath("yyyy-MM-dd_HH-mm-ss-zzz_id");
        break;
    case CodeNaming::FolderRule::PerDay:
        root = QDir(root).filePath(QDate::currentDate().toString("yyyy-MM-dd"));
        break;
    case CodeNaming::FolderRule::Fixed:
        break;
    }
    QString name = mFilenameRule->currentData().toInt() == int(CodeNaming::FilenameRule::Timestamp)
            ? "code_yyyy-MM-dd_HH-mm-ss-zzz_1" : "untitled1";
    name += "." + mDefaultSuffix();
    mPreview->setText(tr("Example path: %1").arg(QDir::toNativeSeparators(QDir(root).filePath(name))));
}
