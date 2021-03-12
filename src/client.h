// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QTimer>
#include <QObject>

#include <memory>

class Client : public QObject
{
    Q_OBJECT

    class Private;
    std::unique_ptr<Private> d;

    QTimer* m_poller;

public:
    Client();
    ~Client();

    Q_SIGNAL void loggedIn();
    Q_SIGNAL void loggedOut();

    Q_SIGNAL void phoneNumberRequested();
    Q_SIGNAL void codeRequested();
    Q_SIGNAL void passwordRequested();

    Q_INVOKABLE void enterPhoneNumber(const QString& phoneNumber);
    Q_INVOKABLE void enterCode(const QString& code);
    Q_INVOKABLE void enterPassword(const QString& password);
};