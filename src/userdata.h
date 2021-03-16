#pragma once

#include <QObject>
#include <QQmlParserStatus>

#include "defs.h"
#include "client.h"
#include "internallib/qabstractrelationalmodel.h"

class Client;

class UserDataModel : public QAbstractRelationalModel
{
    Q_OBJECT

    Client* c;
    std::map<std::int32_t,TDApi::object_ptr<TDApi::user>> m_userData;

public:
    UserDataModel(Client* parent);
    ~UserDataModel();

    void handleUpdate(TDApi::object_ptr<TDApi::Update> u);

    QVariant data(const QVariant& key, int role = Qt::DisplayRole) override;
    bool checkKey(const QVariant& key) override;
    bool canFetchKey(const QVariant& key) override;
    void fetchKey(const QVariant& key) override;

    QHash<int, QByteArray> roleNames() override;
};
