// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QImage>
#include <QImageIOHandler>
#include <QByteArray>
#include <QTemporaryFile>

#include "rlottie.h"

class LottieHandler;

struct LazyImageData
{
    int _frame;
    LottieHandler* _parent;
    QRgb* _data;
    bool _rendered;

    QRgb* data();
};

class LottieHandler : public QImageIOHandler
{

    bool load(QIODevice* it) const;

    mutable bool loaded = false;
    mutable QScopedPointer<rlottie::Animation> animation;
    mutable QList<LazyImageData> imageData;
    mutable QTemporaryFile mmappedFile;
    mutable int currentFrame = -1;

    friend struct LazyImageData;

public:
    LottieHandler();
    ~LottieHandler();

    bool canRead() const override;
    bool read(QImage *image) override;

    QVariant option(ImageOption option) const override;
    void setOption(ImageOption option, const QVariant &value) override;
    bool supportsOption(ImageOption option) const override;

    int imageCount() const override;
    int loopCount() const override;
    int nextImageDelay() const override;
    int currentImageNumber() const override;
};