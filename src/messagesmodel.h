// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QAbstractListModel>

#include "client.h"
#include "internallib/qabstractrelationalmodel.h"
#include "yoinked from qt automotive/qivipendingreply.h"

class QQuickTextDocument;
class QQuickItem;

class MessagesStore : public TokAbstractRelationalModel
{
    Q_OBJECT

    Client* c;

    struct Private;
    std::unique_ptr<Private> d;

public:
    explicit MessagesStore(Client* parent);
    ~MessagesStore();

    void newMessage(TDApi::object_ptr<TDApi::message> msg);
    void messageIDChange(TDApi::int53 oldID, TDApi::object_ptr<TDApi::message> msg);
    void deletedMessages(TDApi::int53 chatID, const TDApi::array<TDApi::int53>& msgIDs);

    Q_INVOKABLE bool format(const QVariant& key, QQuickTextDocument* doc, QQuickItem* it, bool emojiOnly);
    Q_INVOKABLE void applyFormat(const QVariant& key, QQuickTextDocument* to, QQuickItem* it, int fromChar, int toChar);

    QVariant data(const QVariant& key, int role = Qt::DisplayRole) override;
    bool checkKey(const QVariant& key) override;
    bool canFetchKey(const QVariant& key) override;
    void fetchKey(const QVariant& key) override;

    QHash<int, QByteArray> roleNames() override;

    Q_INVOKABLE void deleteMessages(const QString& chatID, const QStringList& messageID);
    Q_INVOKABLE void deleteMessage(const QString& chatID, const QString& messageID, bool deleteForAll);
    Q_INVOKABLE void openVideo(const QString& chatID, const QString& messageID);
};

class MessagesModel : public QAbstractListModel
{
    Q_OBJECT

    Client* c;

    struct Private;
    std::unique_ptr<Private> d;

    struct SendData;
    void send(SendData data);

public:
    explicit MessagesModel(Client* parent, TDApi::int53 id);
    ~MessagesModel();

    void fetch();
    void fetchBack();
    void newMessage(TDApi::int53 msgID);
    void deletedMessages(const TDApi::array<TDApi::int53>& msgIDs);
    void messageIDChanged(TDApi::int53 oldID, TDApi::int53 newID);

    bool canFetchMore(const QModelIndex& parent) const override;
    void fetchMore(const QModelIndex& parent) override;

    QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QHash<int,QByteArray> roleNames() const override;

    Q_INVOKABLE [[deprecated]] void send(QQuickTextDocument* doku, const QString& inReplyTo);
    Q_INVOKABLE [[deprecated]] void sendFile(QQuickTextDocument* doku, QUrl url, const QString& inReplyTo);
    Q_INVOKABLE [[deprecated]] void sendPhoto(QQuickTextDocument* doku, QUrl url, const QString& inReplyTo);

    Q_INVOKABLE void sendAttachment(QQuickTextDocument* doku, QUrl url, const QString& kind);

    Q_INVOKABLE void edit(QQuickTextDocument* doku, const QString& messageID);

    Q_INVOKABLE void messagesInView(QVariantList list);
    Q_INVOKABLE void comingIn();
    Q_INVOKABLE void comingOut();

    Q_INVOKABLE QIviPendingReplyBase hopBackToMessage(const QString& id);
};
