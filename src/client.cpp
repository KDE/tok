// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

#include "client_p.h"
#include "keys.h"

Client::Client()
    : d(new Private(this))
{

    m_poller = new QTimer;

    QObject::connect(m_poller, &QTimer::timeout, m_poller, [this] {
        d->poll();
    });
    m_poller->start(100);
}

Client::~Client()
{
}

void Client::sendQuery(TDApi::object_ptr<TDApi::Function> fn, std::function<void(TObject)> handler)
{
    d->sendQuery(std::move(fn), handler);
}

void Client::enterCode(const QString& code)
{
    d->enterCode(code);
}

void Client::enterPassword(const QString& password)
{
    d->enterPassword(password);
}

void Client::enterPhoneNumber(const QString& phoneNumber)
{
    d->enterPhoneNumber(phoneNumber);
}
