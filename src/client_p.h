// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include "client.h"
#include "chatsmodel.h"
#include "chatsstore.h"
#include "userdata.h"
#include "notificationmanager.h"
#include "defs.h"
#include "messagesmodel.h"

class Client::Private
{
    friend class Client;
    Client* q;

    bool online;

    // members

    TDApi::int53 m_ownID;
    std::unique_ptr<TD::ClientManager> m_clientManager = nullptr;
    std::int32_t m_clientID = 0;
    std::uint64_t m_queryID = 0;
    std::map<std::uint64_t, std::function<void(TObject)>> m_handlers;

    bool m_loggedIn = false;
    std::uint64_t m_authQueryID = 0;
    TDApi::object_ptr<TDApi::AuthorizationState> m_authState;

    // child managers

    std::unique_ptr<ChatsModel> m_chatsModel;
    std::unique_ptr<ChatsStore> m_chatsStore;
    std::unique_ptr<UserDataModel> m_userDataModel;
    std::unique_ptr<MessagesStore> m_messagesStore;
    std::unique_ptr<NotificationManager> m_notificationManager;
    std::map<TDApi::int53, std::unique_ptr<MessagesModel>> m_messageModels;

    // functions

    Private(Client* parent);

    void poll();

    std::uint64_t nextQueryID();
    void sendQuery(TDApi::object_ptr<TDApi::Function> fn, std::function<void(TObject)> handler);
    void checkAuthError(TObject object);
    std::function<void(TObject)> createAuthQueryHandler();
    void handleAuthorizationStateUpdate(TDApi::updateAuthorizationState& upd_state);
    void handleUpdate(TDApi::object_ptr<TDApi::Object> update);
    void handleResponse(TD::ClientManager::Response response);

    void enterPhoneNumber(const QString& phoneNumber);
    void enterCode(const QString& code);
    void enterPassword(const QString& password);
};