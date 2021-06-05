// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QQmlParserStatus>
#include <td/telegram/td_api.h>

#include "defs.h"
#include "client.h"
#include "internallib/qabstractrelationalmodel.h"

class Client;

class UserDataModel : public TokAbstractRelationalModel
{
    Q_OBJECT

    Client* c;

public:
    UserDataModel(Client* parent);
    ~UserDataModel();

    std::map<std::int32_t,TDApi::object_ptr<TDApi::user>> userData;
    std::map<std::int32_t,TDApi::object_ptr<TDApi::userFullInfo>> fullUserData;

    void handleUpdate(TDApi::object_ptr<TDApi::Update> u);

    QVariant data(const QVariant& key, int role = Qt::DisplayRole) override;
    bool checkKey(const QVariant& key) override;
    bool canFetchKey(const QVariant& key) override;
    void fetchKey(const QVariant& key) override;

    QHash<int, QByteArray> roleNames() override;
};
