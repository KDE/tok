// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

#include "client_p.h"
#include "messagesmodel.h"
#include "keys.h"
#include "userdata.h"

Client* gC = nullptr;

Client::Client(bool testing)
    : d(new Private(this))
    , testing(testing)
{
    gC = this;
    d->m_chatsModel.reset(new ChatsModel(this));
    d->m_chatsStore.reset(new ChatsStore(this));
    d->m_userDataModel.reset(new UserDataModel(this));
    d->m_messagesStore.reset(new MessagesStore(this));
    d->m_notificationManager.reset(new NotificationManager(this));
    d->m_fileMangler.reset(new FileMangler(this));
    d->m_chatListModel.reset(new ChatListModel(this));

    d->poll();

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

UserDataModel* Client::userDataModel() const
{
    return d->m_userDataModel.get();
}

MessagesStore* Client::messagesStore() const
{
    return d->m_messagesStore.get();
}

ChatsStore* Client::chatsStore() const
{
    return d->m_chatsStore.get();
}

FileMangler* Client::fileMangler() const
{
    return d->m_fileMangler.get();
}

ChatListModel* Client::chatListModel() const
{
    return d->m_chatListModel.get();
}

bool Client::online() const
{
    return d->online;
}

MembersModel* Client::membersModel(qint64 number, const QString& kind)
{
    if (kind == "basicGroup") {
        return new MembersModel(this, number, false);
    } else if (kind == "superGroup") {
        return new MembersModel(this, number, true);
    }

    return nullptr;
}

ContactsModel* Client::newContactsModel()
{
    return new ContactsModel(this);
}

void Client::setOnline(bool online)
{
    d->online = online;
    call<TDApi::setOption>([](TDApi::setOption::ReturnType) {}, "online", TDApi::make_object<TDApi::optionValueBoolean>(online));
}

void Client::logOut()
{
    call<TDApi::logOut>(nullptr);
}
