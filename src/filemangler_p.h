#pragma once

#include "filemangler.h"

struct FileMangler::Private
{
    QMap<qint32, QSharedPointer<TDApi::file>> fileData;
};
