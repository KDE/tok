// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QImageIOPlugin>

class LottiePlugin : public QImageIOPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QImageIOHandlerFactoryInterface" FILE "rlottie.json")

public:
    Capabilities capabilities(QIODevice *device, const QByteArray &format) const override;
    QImageIOHandler *create(QIODevice *device, const QByteArray &format = QByteArray()) const override;
};
