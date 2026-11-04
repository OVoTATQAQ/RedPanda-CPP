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
#include "codenaming.h"
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QUuid>

QString CodeNaming::createSessionDirectory(const QString &root, QString *error)
{
    QDir parent(root);
    if (!parent.mkpath(".")) {
        if (error) *error = root;
        return {};
    }
    for (int i = 0; i < 10; ++i) {
        QString name = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss-zzz")
                + "_" + QUuid::createUuid().toString(QUuid::WithoutBraces).left(8);
        if (parent.mkdir(name))
            return parent.absoluteFilePath(name);
    }
    if (error) *error = root;
    return {};
}

QString CodeNaming::safeStem(const QString &suggestion)
{
    QString stem = suggestion.trimmed();
    // Treat model output as a name, never as a path or command.
    stem.replace(QRegularExpression("[^A-Za-z0-9_-]+"), "_");
    stem = stem.left(80);
    while (stem.startsWith('_')) stem.remove(0, 1);
    while (stem.endsWith('_')) stem.chop(1);
    static const QRegularExpression reserved("^(CON|PRN|AUX|NUL|COM[0-9]|LPT[0-9])$",
                                             QRegularExpression::CaseInsensitiveOption);
    if (reserved.match(stem).hasMatch()) stem.prepend("code_");
    return stem;
}

QString CodeNaming::reserveFile(const QString &requested, QString *error)
{
    QFileInfo info(requested);
    QDir dir = info.absoluteDir();
    QString suffix = info.suffix().isEmpty() ? QString() : "." + info.suffix();
    for (int i = 0; i < 10000; ++i) {
        QString name = i == 0 ? info.fileName()
                : info.completeBaseName() + "_" + QString::number(i) + suffix;
        QFile file(dir.filePath(name));
        if (file.open(QIODevice::WriteOnly | QIODevice::NewOnly)) {
            file.close();
            return file.fileName();
        }
        if (!QFileInfo::exists(file.fileName()) && !QFileInfo(file.fileName()).isSymLink()) {
            if (error) *error = file.errorString();
            return {};
        }
    }
    if (error) *error = requested;
    return {};
}
