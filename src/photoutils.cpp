// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QDebug>

#include "photoutils.h"

QString imageToURL(const TDApi::object_ptr<TDApi::file>& file)
{
    if (file->local_ != nullptr && file->local_->is_downloading_completed_) {
        return QString("file://" + QString::fromStdString(file->local_->path_));
    }

    return QString("image://telegram/%1").arg(file->id_);
}