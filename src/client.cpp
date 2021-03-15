// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

#include "client_p.h"
#include "messagesmodel.h"
#include "keys.h"

Client* gC = nullptr;

Client::Client()
    : d(new Private(this))
{
    gC = this;
    d->m_chatsModel.reset(new ChatsModel(this));

    m_poller = new QTimer;

    QObject::connect(m_poller, &QTimer::timeout, m_poller, [this] {
        d->poll();
    });
    m_poller->start(0);

}

Client::~Client()
{
}

ChatsModel* Client::chatsModel() const
{
    return d->m_chatsModel.get();
}

MessagesModel* Client::messagesModel(quint64 ID)
{
    if (ID == 0) {
        return nullptr;
    }
    if (!d->m_messageModels.contains(ID)) {
        d->m_messageModels[ID] = std::make_unique<MessagesModel>(this, ID);
    }
    return d->m_messageModels[ID].get();
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

QString Client::ownID() const
{
    return QString::number(d->m_ownID);
}

TDApi::user* Client::userData(qint32 ID)
{
    if (!d->m_users.contains(ID)) {
        return nullptr;
    }

    return d->m_users[ID].get();
}
