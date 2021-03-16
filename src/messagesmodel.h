#pragma once

#include <QAbstractListModel>

#include "client.h"
#include "internallib/qabstractrelationalmodel.h"

class MessagesStore : public QAbstractRelationalModel
{
    Q_OBJECT

    Client* c;

    struct Private;
    std::unique_ptr<Private> d;

public:
    explicit MessagesStore(Client* parent);
    ~MessagesStore();

    void newMessage(TDApi::object_ptr<TDApi::message> msg);

    QVariant data(const QVariant& key, int role = Qt::DisplayRole) override;
    bool checkKey(const QVariant& key) override;
    bool canFetchKey(const QVariant& key) override;
    void fetchKey(const QVariant& key) override;

    QHash<int, QByteArray> roleNames() override;
};

class MessagesModel : public QAbstractListModel
{
    Q_OBJECT

    Client* c;

    struct Private;
    std::unique_ptr<Private> d;

public:
    explicit MessagesModel(Client* parent, TDApi::int53 id);
    ~MessagesModel();

    void fetch();
    void newMessage(TDApi::int53 msgID);

    bool canFetchMore(const QModelIndex& parent) const override;
    void fetchMore(const QModelIndex& parent) override;

    QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QHash<int,QByteArray> roleNames() const override;

    Q_INVOKABLE void send(const QString& contents);
    Q_INVOKABLE void messagesInView(QVariantList list);
    Q_INVOKABLE void comingIn();
    Q_INVOKABLE void comingOut();
};
