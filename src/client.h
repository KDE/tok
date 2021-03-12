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

class Client : public QObject
{
    Q_OBJECT

    class Private;
    std::unique_ptr<Private> d;

    QTimer* m_poller;

private:
    void sendQuery(TDApi::object_ptr<TDApi::Function> fn, std::function<void(TObject)> handler);

public:
    Client();
    ~Client();

    template<typename Fn, typename ...Args>
    void call(std::function<void(typename Fn::ReturnType)> cb, Args ... args) {
        static_assert(std::is_convertible<Fn*, TDApi::Function*>::value, "Derived must be a subclass of TDApi::Function");

        sendQuery(TDApi::make_object<Fn>(args...), [cb](TObject t) {
            auto object = typename Fn::ReturnType(static_cast<typename Fn::ReturnType::pointer>(t.release()));
            cb(std::move(object));
        });
    }

    Q_SIGNAL void loggedIn();
    Q_SIGNAL void loggedOut();

    Q_SIGNAL void phoneNumberRequested();
    Q_SIGNAL void codeRequested();
    Q_SIGNAL void passwordRequested();

    Q_INVOKABLE void enterPhoneNumber(const QString& phoneNumber);
    Q_INVOKABLE void enterCode(const QString& code);
    Q_INVOKABLE void enterPassword(const QString& password);
};