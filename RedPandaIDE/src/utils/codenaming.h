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
#ifndef CODENAMING_H
#define CODENAMING_H
#include <QString>
namespace CodeNaming {
enum class FolderRule { PerSession = 0, PerDay = 1, Fixed = 2 };
enum class FilenameRule { Numbered = 0, Timestamp = 1 };
QString defaultSaveRoot();
QString createSaveDirectory(const QString &root, FolderRule rule, QString *error = nullptr);
QString defaultFileStem(FilenameRule rule, int number);
QString createSessionDirectory(const QString &root, QString *error = nullptr);
QString safeStem(const QString &suggestion);
// Atomically reserve an unused destination. The caller must remove it on failure.
QString reserveFile(const QString &requested, QString *error = nullptr);
}
#endif
