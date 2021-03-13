// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include "client.h"
#include "chatsmodel.h"
#include "defs.h"

class Client::Private
{
    friend class Client;
    Client* q;

    // members

    std::unique_ptr<TD::ClientManager> m_clientManager = nullptr;
    std::int32_t m_clientID = 0;
    std::uint64_t m_queryID = 0;
    std::map<std::uint64_t, std::function<void(TObject)>> m_handlers;

    bool m_loggedIn = false;
    std::uint64_t m_authQueryID = 0;
    TDApi::object_ptr<TDApi::AuthorizationState> m_authState;

    std::map<std::int32_t, TDApi::object_ptr<TDApi::user>> m_users;

    // child managers

    std::unique_ptr<ChatsModel> m_chatsModel;
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