// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QAbstractListModel>

#include "client.h"
#include "yoinked from qt automotive/qivipendingreply.h"

class ChatsModel : public QAbstractListModel
{
    Q_OBJECT

    Client* c;

private:
    void updateChat(TDApi::object_ptr<TDApi::chat> c);
    void newChat(TDApi::object_ptr<TDApi::chat> c);
    void updatePositions(TDApi::int53 chatID, TDApi::object_ptr<TDApi::chatPosition>& pos);

public:

    struct Private;
    std::unique_ptr<Private> d;

    explicit ChatsModel(Client* parent);
    ~ChatsModel();

    void fetch();
    void handleUpdate(TDApi::object_ptr<TDApi::Update> u);

    bool canFetchMore(const QModelIndex& parent) const override;
    void fetchMore(const QModelIndex& parent) override;

    QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QHash<int,QByteArray> roleNames() const override;

    Q_INVOKABLE QIviPendingReplyBase createChat(const QString& name, const QString& type, const QStringList& ids);
    Q_INVOKABLE QIviPendingReplyBase createSecretChat(const QString& withUser);
    Q_INVOKABLE QIviPendingReplyBase createPrivateChat(const QString& withUser);
};
