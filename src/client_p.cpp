// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QtConcurrent>

#include "client_p.h"
#include "keys.h"
#include "overloader.h"
#include "messagesmodel_p.h"
#include "util.h"
#include <qstandardpaths.h>
#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>

#ifdef Q_OS_LINUX
#include <QGuiApplication>
#include <QDBusMessage>
#include <QDBusConnection>

inline auto setUnity(const QVariantMap& props)
{
    QDBusMessage message = QDBusMessage::createSignal("/", "com.canonical.Unity.LauncherEntry", "Update");

    message.setArguments({QGuiApplication::desktopFileName(), props});

    QDBusConnection::sessionBus().send(message);
}

#endif

std::uint64_t Client::Private::nextQueryID()
{
    return ++m_queryID;
}

void Client::Private::sendQuery(TDApi::object_ptr<TDApi::Function> fn, std::function<void(TObject)> handler)
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
            [this](TDApi::authorizationStateReady& ready) {
                m_loggedIn = true;

                m_chatsModel->fetch();
                q->call<TDApi::getInstalledStickerSets>(nullptr, false);
                q->call<TDApi::getOption>([this](TDApi::getOption::ReturnType ret) {
                    m_ownID = static_cast<TDApi::optionValueInteger*>(ret.get())->value_;

                    Q_EMIT q->loggedIn();
                }, "my_id");
                if (q->testing) {
                    Q_EMIT q->loggedIn();
                }
            },
            [this](TDApi::authorizationStateLoggingOut&) {
                m_loggedIn = false;

                Q_EMIT q->loggedOut();
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
                sendQuery(TDApi::make_object<TDApi::checkDatabaseEncryptionKey>(std::move(key)), createAuthQueryHandler());
            },
            [this](TDApi::authorizationStateWaitTdlibParameters&) {
                auto parameters = TDApi::make_object<TDApi::tdlibParameters>();

                const auto appdataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
                const auto tokLocation = QDir::cleanPath(appdataLocation + QDir::separator() + "org.kde.Tok");

                q->call<TDApi::setOption>([](TDApi::setOption::ReturnType) {}, "notification_group_count_max", TDApi::make_object<TDApi::optionValueInteger>(5));
                q->call<TDApi::setOption>([](TDApi::setOption::ReturnType) {}, "notification_group_size_max", TDApi::make_object<TDApi::optionValueInteger>(10));

                parameters->database_directory_ = tokLocation.toStdString();
                parameters->use_message_database_ = true;
                parameters->use_secret_chats_ = true;
                parameters->api_id_ = APP_ID;
                parameters->api_hash_ = API_HASH;
                parameters->system_language_code_ = QLocale::system().name().toStdString();
                parameters->device_model_ = "Desktop";
                parameters->application_version_ = "1.0";
                parameters->enable_storage_optimizer_ = true;
                sendQuery(TDApi::make_object<TDApi::setTdlibParameters>(std::move(parameters)), createAuthQueryHandler());
            }));
}

void Client::Private::handleUpdate(TDApi::object_ptr<TDApi::Object> update)
{
    TDApi::downcast_call(*update,
        overloaded(
            [this, &update](TDApi::updateActiveNotifications& upd) {
                auto mv = TDApi::move_object_as<TDApi::updateActiveNotifications>(update);
                m_notificationManager->handleUpdateActiveNotifications(std::move(mv));
            },
            [this, &update](TDApi::updateNotificationGroup& upd) {
                auto mv = TDApi::move_object_as<TDApi::updateNotificationGroup>(update);
                m_notificationManager->handleUpdateNotificationGroup(std::move(mv));
            },
            [this, &update](TDApi::updateNotification& upd) {
                auto mv = TDApi::move_object_as<TDApi::updateNotification>(update);
                m_notificationManager->handleUpdateNotification(std::move(mv));
            },
            [this, &update](TDApi::updateHavePendingNotifications& upd) {
                auto mv = TDApi::move_object_as<TDApi::updateHavePendingNotifications>(update);
                m_notificationManager->handleUpdateHavePendingNotifications(std::move(mv));
            },
            [this](TDApi::updateAuthorizationState& upd_state) {
                handleAuthorizationStateUpdate(upd_state);
            },
            [this, &update](TDApi::updateNewChat &update_new_chat) {
                auto mv = TDApi::move_object_as<TDApi::Update>(update);
                q->chatsModel()->handleUpdate(std::move(mv));
            },
            [this, &update](TDApi::updateChatTitle &update_chat_title) {
                auto mv = TDApi::move_object_as<TDApi::Update>(update);
                q->chatsModel()->handleUpdate(std::move(mv));
            },
            [this, &update](TDApi::updateChatLastMessage &update_chat_last_message) {
                auto mv = TDApi::move_object_as<TDApi::Update>(update);
                q->chatsModel()->handleUpdate(std::move(mv));
            },
            [this, &update](TDApi::updateChatPosition &update_chat_pos) {
                auto mv = TDApi::move_object_as<TDApi::Update>(update);
                q->chatsModel()->handleUpdate(std::move(mv));
            },
            [this, &update](TDApi::updateUser &user) {
                auto mv = TDApi::move_object_as<TDApi::Update>(update);
                m_userDataModel->handleUpdate(std::move(mv));
            },
            [this, &update](TDApi::updateChatAction &update_user_chat_action) {
                auto mv = TDApi::move_object_as<TDApi::Update>(update);
                q->chatsModel()->handleUpdate(std::move(mv));
            },
            [this, &update](TDApi::updateInstalledStickerSets &update_sticker_sets) {
                auto mv = TDApi::move_object_as<TDApi::updateInstalledStickerSets>(update);
                q->stickerSetsModel()->handleUpdate(std::move(mv));
            },
            [this, &update](TDApi::updateStickerSet &update_sticker_set) {
                auto mv = TDApi::move_object_as<TDApi::updateStickerSet>(update);
                q->stickerSetsStore()->handleUpdate(std::move(mv));
            },
#ifdef Q_OS_LINUX
            [](TDApi::updateUnreadMessageCount& it) {
                if (it.chat_list_->get_id() != TDApi::chatListMain::ID) {
                    return;
                }
                setUnity({
                    {"count-visible", it.unread_count_ > 0},
                    {"count", it.unread_count_},
                });
            },
#endif
            [this](TDApi::updateChatFilters& filts) {
                m_chatListModel->handleUpdate(std::move(filts.chat_filters_));
            },
            [this](TDApi::updateNewMessage &msg) {
                if (!m_messageModels.contains(msg.message_->chat_id_)) {
                    return;
                }
                m_messageModels[msg.message_->chat_id_]->newMessage(msg.message_->id_);
                m_messagesStore->newMessage(std::move(msg.message_));
            },
            [this](TDApi::updateDeleteMessages &msgs) {
                if (!m_messageModels.contains(msgs.chat_id_) || msgs.from_cache_) {
                    return;
                }
                m_messageModels[msgs.chat_id_]->deletedMessages(msgs.message_ids_);
                m_messagesStore->deletedMessages(msgs.chat_id_, msgs.message_ids_);
            },
            [this](TDApi::updateMessageSendSucceeded &msg) {
                if (!m_messageModels.contains(msg.message_->chat_id_)) {
                    return;
                }
                m_messageModels[msg.message_->chat_id_]->messageIDChanged(msg.old_message_id_, msg.message_->id_);
                m_messagesStore->messageIDChange(msg.old_message_id_, std::move(msg.message_));
            },
            [this](TDApi::updateFile &file) {
                auto ptr = QSharedPointer<TDApi::file>(file.file_.release());
                Q_EMIT q->fileDataChanged(ptr->id_, ptr);
            },
            [this, &update](TDApi::updateChatReadInbox &upd) {
                auto mv = TDApi::move_object_as<TDApi::Update>(update);
                q->chatsModel()->handleUpdate(std::move(mv));
            },
            [this, &update](TDApi::updateChatPermissions &upd) {
                auto mv = TDApi::move_object_as<TDApi::Update>(update);
                q->chatsModel()->handleUpdate(std::move(mv));
            },
            [this](TDApi::updateConnectionState &upd) {
                switch (upd.state_->get_id()) {
                case TDApi::connectionStateConnecting::ID: connectionState = Client::Connecting; break;
                case TDApi::connectionStateConnectingToProxy::ID: connectionState = Client::ConnectingToProxy; break;
                case TDApi::connectionStateReady::ID: connectionState = Client::Ready; break;
                case TDApi::connectionStateUpdating::ID: connectionState = Client::Updating; break;
                case TDApi::connectionStateWaitingForNetwork::ID: connectionState = Client::WaitingForNetwork; break;
                }
                Q_EMIT q->connectionStateChanged();
            },
            [](auto& update) { /* qWarning() << "unhandled private client update" << QString::fromStdString(TDApi::to_string(update)); */ }));
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
    if (q->testing) {
        return;
    }
    QtConcurrent::run([this] {
        while (!quitting) {
            auto response = m_clientManager->receive(10);

            if (response.object) {
                runOnMainThread([this, resp = new TD::ClientManager::Response(std::move(response))] {
                    auto mu = TD::ClientManager::Response(std::move(*resp));
                    delete resp;
                    handleResponse(std::move(mu));
                });
            }
        }
    });
}

Client::Private::Private(Client* parent)
    : q(parent)
{
    TD::ClientManager::execute(TDApi::make_object<TDApi::setLogVerbosityLevel>(1));
    m_clientManager = std::make_unique<TD::ClientManager>();
    m_clientID = m_clientManager->create_client_id();

    sendQuery(TDApi::make_object<TDApi::getOption>("version"), {});
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, parent, [this](){
#ifdef Q_OS_LINUX
        setUnity({
            {"count-visible", false},
            {"count", 0},
        });
#endif
        quitting = true;
    });
}

void Client::Private::enterPhoneNumber(const QString& phoneNumber)
{
    sendQuery(TDApi::make_object<TDApi::setAuthenticationPhoneNumber>(phoneNumber.toStdString(), nullptr), createAuthQueryHandler());
}

void Client::Private::enterCode(const QString& code)
{
    sendQuery(TDApi::make_object<TDApi::checkAuthenticationCode>(code.toStdString()), createAuthQueryHandler());
}

void Client::Private::enterPassword(const QString& password)
{
    sendQuery(TDApi::make_object<TDApi::checkAuthenticationPassword>(password.toStdString()), createAuthQueryHandler());
}
