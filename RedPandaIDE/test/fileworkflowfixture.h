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
#ifndef FILEWORKFLOWFIXTURE_H
#define FILEWORKFLOWFIXTURE_H

#include "src/addon/fileworkflowloader.h"
#include "src/editor.h"
#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QSettings>
#include <QTemporaryDir>

struct FileWorkflowFixture {
    QMainWindow window;
    QMenu viewMenu;
    FileWorkflowLoader loader;

    explicit FileWorkflowFixture(const QTemporaryDir &root, Editor *editor = nullptr) :
        loader({&window, &viewMenu, root.filePath("settings.ini"),
                [editor]() -> StandaloneFile {
                    if (!editor || editor->inProject()) return {};
                    return {editor, editor->suggestedSaveName(), editor->text()};
                },
                [editor](QObject *source, const QString &name) {
                    return editor && editor == source && editor->saveAs(name, false);
                },
                []() -> QString { return "cpp"; }}, nullptr, SESSION_AI_PLUGIN_FILE)
    {
        QSettings settings(root.filePath("settings.ini"), QSettings::IniFormat);
        if (!settings.contains("Editor/new_file_save_root"))
            settings.setValue("Editor/new_file_save_root", root.filePath("code"));
        if (editor)
            QObject::connect(&loader, &FileWorkflowLoader::activeChanged, editor, &Editor::setFileWorkflow);
        loader.restore();
    }

    void enable() { loader.toggleAction()->setChecked(true); }
    void disable() { loader.toggleAction()->setChecked(false); }

    void newFile(Editor *editor, int number = 1) {
        editor->setFilename(QString("untitled%1.cpp").arg(number));
        editor->setIsNew(true);
        if (auto plugin = loader.active())
            plugin->newFile(editor->document().get(), editor->filename(), number);
    }
};

#endif
