// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "client_p.h"
#include "keys.h"
#include <td/telegram/td_api.h>

// helper function yoinked from tdlib example
namespace detail
{
template<class... Fs>
struct overload;

template<class F>
struct overload<F> : public F {
    explicit overload(F f)
        : F(f)
    {
    }
};
template<class F, class... Fs>
struct overload<F, Fs...> : public overload<F>, overload<Fs...> {
    overload(F f, Fs... fs)
        : overload<F>(f)
        , overload<Fs...>(fs...)
    {
    }
    using overload<F>::operator();
    using overload<Fs...>::operator();
};
}

template<class... F>
auto overloaded(F... f)
{
    return detail::overload<F...>(f...);
}

std::uint64_t Client::Private::nextQueryID()
{
    return ++m_queryID;
}

void Client::Private::sendQuery(TDApi::object_ptr<TDApi::Function> fn,
    std::function<void(TObject)> handler)
{
    auto queryID = nextQueryID();
    if (handler) {
        m_handlers.emplace(queryID, handler);
    }
    m_clientManager->send(m_clientID, queryID, std::move(fn));
}

void Client::Private::checkAuthError(TObject object)
{
    if (object->get_id() == TDApi::error::ID) {
        auto error = td::move_tl_object_as<TDApi::error>(object);
        qWarning() << "Error:" << QString::fromStdString(error->message_);
    }
}

std::function<void(TObject)> Client::Private::createAuthQueryHandler()
{
    return [this, id = m_authQueryID](TObject object) {
        if (id == m_authQueryID) {
            checkAuthError(std::move(object));
        }
    };
}

void Client::Private::handleAuthorizationStateUpdate(TDApi::updateAuthorizationState& upd_state)
{
    m_authState = std::move(upd_state.authorization_state_);
    TDApi::downcast_call(*m_authState,
        overloaded(
            [this](TDApi::authorizationStateReady&) {
                m_loggedIn = true;
            },
            [this](TDApi::authorizationStateLoggingOut&) {
                m_loggedIn = false;
            },
            [](TDApi::authorizationStateClosing&) {
                qDebug() << "Closing";
            },
            [this](TDApi::authorizationStateClosed&) {
                m_loggedIn = false;
                // TODO: apparently we're supposed to restart?
            },
            [this](TDApi::authorizationStateWaitCode&) {
                Q_EMIT q->codeRequested();
                // TODO: prompt for code in UI
            },
            [](TDApi::authorizationStateWaitRegistration&) {
                // TODO: prompt for this in UI
            },
            [this](TDApi::authorizationStateWaitPassword&) {
                Q_EMIT q->passwordRequested();
                // TODO: prompt for password in UI
            },
            [](TDApi::authorizationStateWaitOtherDeviceConfirmation& state) {
                // TODO: show QR code in UI
            },
            [this](TDApi::authorizationStateWaitPhoneNumber&) {
                Q_EMIT q->phoneNumberRequested();
                // TODO: prompt for phone number
            },
            [this](TDApi::authorizationStateWaitEncryptionKey&) {
                // TODO: prompt for encryption key
                auto key = "";
                sendQuery(TDApi::make_object<TDApi::checkDatabaseEncryptionKey>(std::move(key)),
                    createAuthQueryHandler());
            },
            [this](TDApi::authorizationStateWaitTdlibParameters&) {
                auto parameters = TDApi::make_object<TDApi::tdlibParameters>();
                parameters->database_directory_ = "tdlib";
                parameters->use_message_database_ = true;
                parameters->use_secret_chats_ = false;
                parameters->api_id_ = APP_ID;
                parameters->api_hash_ = API_HASH;
                parameters->system_language_code_ = QLocale::system().language();
                parameters->device_model_ = "Desktop";
                parameters->application_version_ = "1.0";
                parameters->enable_storage_optimizer_ = true;
                sendQuery(TDApi::make_object<TDApi::setTdlibParameters>(std::move(parameters)),
                    createAuthQueryHandler());
            }));
}

void Client::Private::handleUpdate(TDApi::object_ptr<TDApi::Object> update)
{
    TDApi::downcast_call(*update,
        overloaded(
            [this](TDApi::updateAuthorizationState& upd_state) {
                handleAuthorizationStateUpdate(upd_state);
            },
            [](auto& update) { /* fallback */ }));
}

void Client::Private::handleResponse(TD::ClientManager::Response response)
{
    if (!response.object) {
        return;
    }

    if (response.request_id == 0) {
        handleUpdate(std::move(response.object));
    }

    auto it = m_handlers.find(response.request_id);
    if (it != m_handlers.end()) {
        it->second(std::move(response.object));
    }
}

void Client::Private::poll()
{
    auto response = m_clientManager->receive(0);

    if (response.object) {
        handleResponse(std::move(response));
    }
}

Client::Private::Private(Client* parent)
    : q(parent)
{
    TD::ClientManager::execute(TDApi::make_object<TDApi::setLogVerbosityLevel>(1));
    m_clientManager = std::make_unique<TD::ClientManager>();
    m_clientID = m_clientManager->create_client_id();

    sendQuery(TDApi::make_object<TDApi::getOption>("version"), {});
}

void Client::Private::enterPhoneNumber(const QString& phoneNumber)
{
    sendQuery(
        TDApi::make_object<TDApi::setAuthenticationPhoneNumber>(phoneNumber.toStdString(), nullptr),
        createAuthQueryHandler());
}

void Client::Private::enterCode(const QString& code)
{
    sendQuery(TDApi::make_object<TDApi::checkAuthenticationCode>(code.toStdString()),
        createAuthQueryHandler());
}

void Client::Private::enterPassword(const QString& password)
{
    sendQuery(TDApi::make_object<TDApi::checkAuthenticationPassword>(password.toStdString()),
        createAuthQueryHandler());
}
