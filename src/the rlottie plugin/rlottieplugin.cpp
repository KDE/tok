// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "rlottieplugin.h"
#include "rlottieimage.h"

LottiePlugin::Capabilities LottiePlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    Q_UNUSED(device)

    if (format == "lottie" or format == "tgs") {
        return Capabilities(CanRead);
    }

    return {};
}

QImageIOHandler* LottiePlugin::create(QIODevice *device, const QByteArray &format) const
{
    QImageIOHandler *handler = new LottieHandler;
    handler->setDevice(device);
    handler->setFormat(format);
    return handler;
}
