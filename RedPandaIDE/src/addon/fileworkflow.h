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
#ifndef FILEWORKFLOW_H
#define FILEWORKFLOW_H

#include <QPointer>
#include <QString>
#include <QtPlugin>
#include <functional>
#include <optional>

class QMainWindow;
class QMenu;

struct StandaloneFile {
    QPointer<QObject> editor;
    QString filename;
    QString text;
};

struct FileWorkflowContext {
    QMainWindow *window;
    QMenu *viewMenu;
    QString settingsFile;
    std::function<StandaloneFile()> currentFile;
    std::function<bool(QObject *, const QString &)> saveFileAs;
    std::function<QString()> defaultSuffix;
};

class FileWorkflow {
public:
    virtual ~FileWorkflow() = default;
    virtual void activate(const FileWorkflowContext &context, bool showPanel) = 0;
    virtual void deactivate() = 0;

    // Only newly created standalone documents receive this notification.
    virtual void newFile(QObject *document, const QString &filename, int number) = 0;
    virtual QString suggestedPath(const QObject *document) const = 0;
    // An empty result leaves the editor's regular Save/Save As workflow in charge.
    virtual std::optional<bool> firstSave(const QObject *document, bool force,
            const std::function<bool(const QString &)> &saveAs) = 0;
};

#define FileWorkflow_iid "org.redpanda-cpp.FileWorkflow/1.0"
Q_DECLARE_INTERFACE(FileWorkflow, FileWorkflow_iid)
#endif
