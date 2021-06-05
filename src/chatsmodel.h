// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QAbstractListModel>

#include "client.h"

class ChatsModel : public QAbstractListModel
{
    Q_OBJECT

    Client* c;

    struct Private;
    std::unique_ptr<Private> d;

private:
    void updateChat(TDApi::object_ptr<TDApi::chat> c);
    void newChat(TDApi::object_ptr<TDApi::chat> c);
    void updatePositions(TDApi::int53 chatID, TDApi::object_ptr<TDApi::chatPosition>& pos);

public:
    explicit ChatsModel(Client* parent);
    ~ChatsModel();

    void fetch();
    void handleUpdate(TDApi::object_ptr<TDApi::Update> u);

    bool canFetchMore(const QModelIndex& parent) const override;
    void fetchMore(const QModelIndex& parent) override;

    QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QHash<int,QByteArray> roleNames() const override;
};
