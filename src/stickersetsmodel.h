// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QAbstractListModel>

#include "client.h"

class StickerSetsModel : public QAbstractListModel
{
    Q_OBJECT

    Client* c;

public:

    struct Private;
    std::unique_ptr<Private> d;

    explicit StickerSetsModel(Client* parent);
    ~StickerSetsModel();

    void handleUpdate(TDApi::object_ptr<TDApi::updateInstalledStickerSets> u);

    QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QHash<int,QByteArray> roleNames() const override;
};