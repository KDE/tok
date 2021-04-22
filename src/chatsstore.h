#pragma once

#include "client.h"
#include "internallib/qabstractrelationalmodel.h"

class ChatsStore : public TokAbstractRelationalModel
{
    Q_OBJECT

    Client* c;

    struct Private;
    std::unique_ptr<Private> d;

    friend class ChatsModel;

public:
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
};
