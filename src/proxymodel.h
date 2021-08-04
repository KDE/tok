// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QAbstractListModel>

#include "client.h"

class ProxyModel : public QAbstractListModel
{

    Q_OBJECT

    Client* c;

    struct Private;
    std::unique_ptr<Private> d;

public:

    explicit ProxyModel(Client* parent);
    ~ProxyModel();

    QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& idx, const QVariant& data, int role = Qt::DisplayRole) override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QHash<int,QByteArray> roleNames() const override;

    Q_INVOKABLE void insert(const QString& server, int port, bool enabled, const QJsonObject& otherData);

};
