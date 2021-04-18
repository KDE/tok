// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QTimer>

#include <memory>
#include <td/telegram/td_api.h>
#include <td/tl/TlObject.h>

#include "defs.h"

class ChatsModel;
class ChatsStore;
class MessagesModel;
class UserDataModel;
class MessagesStore;

class Client : public QObject
{
    Q_OBJECT

    Q_PROPERTY(ChatsStore* chatsStore READ chatsStore CONSTANT)
    Q_PROPERTY(ChatsModel* chatsModel READ chatsModel CONSTANT)
    Q_PROPERTY(UserDataModel* userDataModel READ userDataModel CONSTANT)
    Q_PROPERTY(MessagesStore* messagesStore READ messagesStore CONSTANT)
    Q_PROPERTY(bool online READ online WRITE setOnline NOTIFY onlineChanged)
    Q_PROPERTY(QString ownID READ ownID CONSTANT)

    class Private;
    std::unique_ptr<Private> d;

private:
    void sendQuery(TDApi::object_ptr<TDApi::Function> fn, std::function<void(TObject)> handler);

public:
    Client();
    ~Client();

    template<typename Fn, typename ...Args>
    void call(std::function<void(typename Fn::ReturnType)> cb, Args ... args) {
        static_assert(std::is_convertible<Fn*, TDApi::Function*>::value, "Derived must be a subclass of TDApi::Function");

        sendQuery(TDApi::make_object<Fn>(std::forward<Args>(args)...), [cb](TObject t) {
            if (t->get_id() == TDApi::error::ID) {
                auto error = TDApi::move_object_as<TDApi::error>(t);
                qDebug() << "Error:" << error->code_ << QString::fromStdString(error->message_);
                return;
            }
            if (cb == nullptr) {
                return;
            }

            auto object = typename Fn::ReturnType(static_cast<typename Fn::ReturnType::pointer>(t.release()));
            cb(std::move(object));
        });
    }

    template<typename Fn>
    void callP(std::function<void(typename Fn::ReturnType)> cb, TDApi::object_ptr<Fn> a) {
        static_assert(std::is_convertible<Fn*, TDApi::Function*>::value, "Derived must be a subclass of TDApi::Function");

        sendQuery(std::move(a), [cb](TObject t) {
            if (t->get_id() == TDApi::error::ID) {
                auto error = TDApi::move_object_as<TDApi::error>(t);
                qDebug() << "Error:" << error->code_ << QString::fromStdString(error->message_);
                return;
            }
            if (cb == nullptr) {
                return;
            }

            auto object = typename Fn::ReturnType(static_cast<typename Fn::ReturnType::pointer>(t.release()));
            cb(std::move(object));
        });
    }

    ChatsModel* chatsModel() const;
    UserDataModel* userDataModel() const;
    ChatsStore* chatsStore() const;
    MessagesStore* messagesStore() const;
    QString ownID() const;

    bool online() const;
    void setOnline(bool online);
    Q_SIGNAL void onlineChanged();

    Q_SIGNAL void loggedIn();
    Q_SIGNAL void loggedOut();

    Q_SIGNAL void phoneNumberRequested();
    Q_SIGNAL void codeRequested();
    Q_SIGNAL void passwordRequested();

    Q_SIGNAL void fileDataChanged(qint32 ID, QSharedPointer<TDApi::file> file);

    MessagesModel* messagesModel(quint64 number);
    Q_INVOKABLE MessagesModel* messagesModel(const QString& s)
    {
        return messagesModel(s.toLongLong());
    }

    Q_INVOKABLE void enterPhoneNumber(const QString& phoneNumber);
    Q_INVOKABLE void enterCode(const QString& code);
    Q_INVOKABLE void enterPassword(const QString& password);
};