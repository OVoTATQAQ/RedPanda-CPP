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
#ifndef FILEWORKFLOWLOADER_H
#define FILEWORKFLOWLOADER_H

#include "fileworkflow.h"
#include <QPluginLoader>

class QAction;

class FileWorkflowLoader : public QObject {
    Q_OBJECT
public:
    FileWorkflowLoader(FileWorkflowContext context, QObject *parent = nullptr,
                       const QString &pluginFile = {});
    ~FileWorkflowLoader() override;

    QAction *toggleAction() const { return mToggle; }
    FileWorkflow *active() const { return mActive; }
    void restore();

signals:
    void activeChanged(FileWorkflow *workflow);
    void loadFailed(const QString &reason);

private:
    bool start(bool showPanel);
    void stop();

    FileWorkflowContext mContext;
    QString mPluginFile;
#ifndef REDPANDA_STATIC_FILE_PLUGIN
    QPluginLoader mLoader;
#endif
    FileWorkflow *mActive = nullptr;
    QAction *mToggle;
};

#endif
