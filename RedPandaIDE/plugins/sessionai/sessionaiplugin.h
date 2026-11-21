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
#ifndef SESSIONAIPLUGIN_H
#define SESSIONAIPLUGIN_H

#include "codenaming.h"
#include "src/addon/fileworkflow.h"
#include <QHash>
#include <QTranslator>

class QDockWidget;

class SessionAiPlugin : public QObject, public FileWorkflow {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID FileWorkflow_iid)
    Q_INTERFACES(FileWorkflow)
public:
    void activate(const FileWorkflowContext &context, bool showPanel) override;
    void deactivate() override;
    void newFile(QObject *document, const QString &filename, int number) override;
    QString suggestedPath(const QObject *document) const override;
    std::optional<bool> firstSave(const QObject *document, bool force,
            const std::function<bool(const QString &)> &saveAs) override;

private:
    QString saveDirectory();

    struct PendingFile {
        QString path;
        bool direct = false;
    };
    QHash<const QObject *, PendingFile> mPendingFiles;
    CodeNaming::Preferences mPreferences;
    QTranslator mTranslator;
    QString mDirectoryError;
    QDockWidget *mDock = nullptr;
};

#endif
