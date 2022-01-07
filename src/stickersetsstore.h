// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "client.h"
#include "internallib/qabstractrelationalmodel.h"

struct Sticker
{

    Q_GADGET

    Q_PROPERTY(qint32 width MEMBER width)
    Q_PROPERTY(qint32 height MEMBER height)
    Q_PROPERTY(QString emoji MEMBER emoji)
    Q_PROPERTY(bool isAnimated MEMBER isAnimated)
    Q_PROPERTY(QString thumbnail MEMBER thumbnail)
    Q_PROPERTY(QString stickerURL MEMBER stickerURL)

    qint32 width;
    qint32 height;
    QString emoji;
    bool isAnimated;
    QString thumbnail;
    QString stickerURL;

public:
    Sticker();
    ~Sticker();

    static Sticker fromTDApi(const TDApi::object_ptr<TDApi::sticker>& sticker);
};

class StickerSetsStore : public TokAbstractRelationalModel
{
    Q_OBJECT

    Client* c;

    void updateSet(TDApi::object_ptr<TDApi::stickerSet>& set);

public:

    struct Private;
    std::unique_ptr<Private> d;

    explicit StickerSetsStore(Client* parent);
    ~StickerSetsStore();

    void handleUpdate(TDApi::object_ptr<TDApi::updateStickerSet> c);

    QVariant data(const QVariant& key, int role = Qt::DisplayRole) override;
    bool checkKey(const QVariant& key) override;
    bool canFetchKey(const QVariant& key) override;
    void fetchKey(const QVariant& key) override;

    QHash<int, QByteArray> roleNames() override;
};
