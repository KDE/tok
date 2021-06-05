// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "tgimageprovider.h"

TelegramImageProviderResponse::TelegramImageProviderResponse(Client* c, std::int32_t image)
{
    auto onFinished = [this, image](qint32 id, QSharedPointer<TDApi::file> file) {
        if (id != image) {
            return;
        }
        if (file->local_ == nullptr) {
            return;
        }
        if (!file->local_->is_downloading_completed_) {
            return;
        }
        m_image = QImage(QString::fromStdString(file->local_->path_));

        Q_EMIT finished();
    };
    c->call<TDApi::downloadFile>(
        [onFinished, it = QPointer(this)](TDApi::downloadFile::ReturnType t) {
            if (it.isNull()) {
                return;
            }
            auto id = t->id_;
            onFinished(id, QSharedPointer<TDApi::file>(t.release()));
        },
        image, 10, 0, 0, false
    );
    connect(c, &Client::fileDataChanged, this, onFinished);
}

TelegramImageProviderResponse::~TelegramImageProviderResponse()
{
}

QQuickTextureFactory* TelegramImageProviderResponse::textureFactory() const
{
    return QQuickTextureFactory::textureFactoryForImage(m_image);
}

QQuickImageResponse* TelegramImageProvider::requestImageResponse(const QString& id, const QSize& requestSize)
{
    return new TelegramImageProviderResponse(c, id.toLong());
}
