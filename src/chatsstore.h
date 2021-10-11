// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "client.h"
#include "internallib/qabstractrelationalmodel.h"

class ChatsStore : public TokAbstractRelationalModel
{
    Q_OBJECT

    Client* c;

    friend class ChatsModel;
    friend class ChatSortModel;

    QJsonValue prepare(TDApi::int53 chat);
    void fetchExtended(TDApi::int53 chat);

public:

    struct Private;
    std::unique_ptr<Private> d;

    explicit ChatsStore(Client* parent);
    ~ChatsStore();

    void newChat(TDApi::object_ptr<TDApi::chat> c);
    void updateChat(TDApi::object_ptr<TDApi::chat> c);
    void handleUpdate(TDApi::object_ptr<TDApi::Update> u);

    QVariant data(const QVariant& key, int role = Qt::DisplayRole) override;
    bool checkKey(const QVariant& key) override;
    bool canFetchKey(const QVariant& key) override;
    void fetchKey(const QVariant& key) override;

    QHash<int, QByteArray> roleNames() override;

    Q_INVOKABLE void setStatus(const QString& chatID, const QString& userID, QJsonObject params);
};
