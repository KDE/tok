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
    d->m_proxyModel.reset(new ProxyModel(this));
    d->m_stickerSetsModel.reset(new StickerSetsModel(this));
    d->m_stickerSetsStore.reset(new StickerSetsStore(this));

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

QIviPendingReplyBase Client::searchEmojis(const QString& emoji)
{
    QIviPendingReply<QStringList> data;

    QLocale l;
    const auto langs = l.uiLanguages();

    TDApi::array<TDApi::string> locales;
    locales.reserve(langs.length());
    for (const auto& lang : langs) {
        locales.push_back(lang.toStdString());
    }

    call<TDApi::searchEmojis>(
        [data](TDApi::searchEmojis::ReturnType r) mutable {
            if (r->emojis_.empty()) {
                data.setSuccess(QStringList{});
                return;
            }

            QStringList it;
            it.reserve(qMax((int)r->emojis_.size(), 5));
            int i = 0;

            for (const auto& em : r->emojis_) {
                if (i >= 5) {
                    break;
                }
                it << QString::fromStdString(em);
                i++;
            }

            data.setSuccess(it);
        },
        emoji.toStdString(), false, locales
    );

    return data;
}

QIviPendingReplyBase Client::searchPublicChat(const QString& username)
{
    QIviPendingReply<QJsonObject> data;

    call<TDApi::searchPublicChat>(
        [data](TDApi::searchPublicChat::ReturnType r) mutable {
            using namespace TDApi;

            QJsonObject it;
            it["id"] = QString::number(r->id_);

            match (r->type_)
                handleCase(chatTypeBasicGroup, basic)
                    it["type"] = "basicgroup";
                    it["chatID"] = QString::number(basic->basic_group_id_);
                endhandle
                handleCase(chatTypeSupergroup, supergroup)
                    it["type"] = "supergroup";
                    it["chatID"] = QString::number(supergroup->supergroup_id_);
                endhandle
                handleCase(chatTypeSecret, secret)
                    it["type"] = "secret";
                    it["chatID"] = QString::number(secret->secret_chat_id_);
                endhandle
                handleCase(chatTypePrivate, priv)
                    it["type"] = "private";
                    it["chatID"] = QString::number(priv->user_id_);
                endhandle
            endmatch

            data.setSuccess(it);
        },
        username.toStdString()
    );

    return data;
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

ProxyModel* Client::proxyModel() const
{
    return d->m_proxyModel.get();
}

StickerSetsModel* Client::stickerSetsModel() const
{
    return d->m_stickerSetsModel.get();
}

StickerSetsStore* Client::stickerSetsStore() const
{
    return d->m_stickerSetsStore.get();
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

bool Client::doNotDisturb() const
{
    return d->dnd;
}

void Client::setDoNotDisturb(bool dnd)
{
    if (d->dnd == dnd) {
        return;
    }

    d->dnd = dnd;
    Q_EMIT doNotDisturbChanged();
}

Client::ConnectionState Client::connectionState() const
{
    return d->connectionState;
}
