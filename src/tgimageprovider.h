// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QQuickAsyncImageProvider>
#include "client.h"

class TelegramImageProviderResponse : public QQuickImageResponse
{
    Q_OBJECT

    QImage m_image;

public:
    TelegramImageProviderResponse(Client* c, std::int32_t image);
    ~TelegramImageProviderResponse();

    QQuickTextureFactory* textureFactory() const override;
};

class TelegramImageProvider : public QQuickAsyncImageProvider
{
    Client* c;

public:
    TelegramImageProvider(Client* c) : QQuickAsyncImageProvider(), c(c)
    {
    }

    QQuickImageResponse* requestImageResponse(const QString& id, const QSize& requestSize) override;
};
