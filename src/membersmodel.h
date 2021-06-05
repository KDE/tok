// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QAbstractListModel>

#include "client.h"

class MembersModel : public QAbstractListModel
{

    Q_OBJECT

    Client* c;

    struct Private;
    std::unique_ptr<Private> d;

public:
    explicit MembersModel(Client* parent, TDApi::int53 chatID, bool isSupergroup);
    ~MembersModel();

    bool canFetchMore(const QModelIndex& parent) const override;
    void fetchMore(const QModelIndex& parent) override;

    QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QHash<int,QByteArray> roleNames() const override;

};
