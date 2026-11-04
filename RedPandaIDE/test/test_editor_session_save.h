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
#ifndef TEST_EDITOR_SESSION_SAVE_H
#define TEST_EDITOR_SESSION_SAVE_H
#include "test_editor_base.h"
class TestEditorSessionSave : public TestEditorBase {
    Q_OBJECT
private slots:
    void saveNewAndUpdate();
    void settingsRoundTrip();
    void invalidRulesUseDefaults();
    void manualFirstSaveCanBeCancelled();
    void collisionKeepsOriginal();
};
#endif
