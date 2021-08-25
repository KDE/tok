/*
   SPDX-FileCopyrightText: 2018 (c) Matthieu Gallien <matthieu_gallien@yahoo.fr>
   SPDX-FileCopyrightText: 2021 (c) Carson Black <uhhadd@gmail.com>

   SPDX-License-Identifier: GPL-3.0-or-later
   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <QFileInfo>
#include <QDir>
#include <QFileInfoList>

#include "album_art_extractinator.h"

// most of this code yoinked from elisa

const static QStringList constSearchStrings = {
    QStringLiteral("*[Cc]over*.jpg")
    ,QStringLiteral("*[Cc]over*.png")
    ,QStringLiteral("*[Ff]older*.jpg")
    ,QStringLiteral("*[Ff]older*.png")
    ,QStringLiteral("*[Ff]ront*.jpg")
    ,QStringLiteral("*[Ff]ront*.png")
    ,QStringLiteral("*[Aa]lbumart*.jpg")
    ,QStringLiteral("*[Aa]lbumart*.png")
    ,QStringLiteral("*[Cc]over*.jpg")
    ,QStringLiteral("*[Cc]over*.png")
};

QUrl searchForCoverFile(const QString &localFileName)
{
    const QFileInfo trackFilePath(localFileName);
    QDir trackFileDir = trackFilePath.absoluteDir();
    trackFileDir.setFilter(QDir::Files);
    trackFileDir.setNameFilters(constSearchStrings);
    QFileInfoList coverFiles = trackFileDir.entryInfoList();
    if (coverFiles.isEmpty()) {
        const QString dirNamePattern = QLatin1String("*") + trackFileDir.dirName() + QLatin1String("*");
        const QString dirNameNoSpaces = QLatin1String("*") + trackFileDir.dirName().remove(QLatin1Char(' ')) + QLatin1String("*");
        const QStringList filters = {
            dirNamePattern + QStringLiteral(".jpg"),
            dirNamePattern + QStringLiteral(".png"),
            dirNameNoSpaces + QStringLiteral(".jpg"),
            dirNameNoSpaces + QStringLiteral(".png")
        };
        trackFileDir.setNameFilters(filters);
        coverFiles = trackFileDir.entryInfoList();
    }
    if (coverFiles.isEmpty()) {
        return QUrl();
    }
    return QUrl::fromLocalFile(coverFiles.first().absoluteFilePath());
}

QUrl extractinateAlbumArt(const QUrl& forFile)
{
    // TODO: extract embedded art from file using KFileMetaData
    return searchForCoverFile(forFile.toLocalFile());
}
