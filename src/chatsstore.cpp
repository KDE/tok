// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <KLocalizedString>

#include "defs.h"
#include "messagesmodel.h"
#include "chatsstore_p.h"
#include "overloader.h"
#include "photoutils.h"

enum Roles {
    Title = Qt::UserRole,
    LastMessageID,
    Photo,
    UnreadCount,
    Kind,
    KindID,
    IsChannel,
    CanSendMessages,
    CanSendMedia, // True, if the user can send audio files, documents, photos, videos, video notes, and voice notes. Implies can_send_messages permissions.
    CanSendPolls,
    CanSendOther, // True, if the user can send animations, games, stickers, and dice and use inline bots. Implies can_send_messages permissions.
    CanSendWebPreview,
    CanChangeInfo,
    CanInviteUsers,
    CanPinMessages,
    CurrentActions,
    HeaderText,
    OwnStatus,
};

ChatsStore::ChatsStore(Client* parent) : TokAbstractRelationalModel(parent), c(parent), d(new Private)
{
}

ChatsStore::~ChatsStore()
{

}

auto to(TDApi::int53 s) { return QString::number(s); }
auto from(const QString& s) { return s.toLongLong(); }
auto from(const QVariant& s) { return from(s.toString()); }

void ChatsStore::newChat(TDApi::object_ptr<TDApi::chat> c)
{
    auto id = c->id_;

    d->chatData[c->id_] = std::move(c);
    Q_EMIT keyAdded(to(id));
}

void ChatsStore::updateChat(TDApi::object_ptr<TDApi::chat> c)
{
    auto id = c->id_;

    d->chatData[c->id_] = std::move(c);
    Q_EMIT keyDataChanged(to(id), {});
}

void ChatsStore::handleUpdate(TDApi::object_ptr<TDApi::Update> u)
{
    TDApi::downcast_call(*u,
        overloaded(
            [this](TDApi::updateNewChat &update_new_chat) {
                newChat(std::move(update_new_chat.chat_));
            },
            [this](TDApi::updateChatTitle &update_chat_title) {
                auto id = update_chat_title.chat_id_;

                d->chatData[id]->title_ = update_chat_title.title_;

                Q_EMIT keyDataChanged(to(id), {});
            },
            [this](TDApi::updateChatLastMessage &update_chat_last_message) {
                if (!update_chat_last_message.last_message_.get()) {
                    return;
                }

                auto id = update_chat_last_message.chat_id_;

                d->chatData[id]->last_message_ = TD::make_tl_object<TDApi::message>();
                d->chatData[id]->last_message_->id_ = update_chat_last_message.last_message_->id_;
                c->messagesStore()->newMessage(std::move(update_chat_last_message.last_message_));

                Q_EMIT keyDataChanged(to(id), {});
            },
            [this](TDApi::updateChatReadInbox &update_chat_read_inbox) {
                auto id = update_chat_read_inbox.chat_id_;

                d->chatData[id]->last_read_inbox_message_id_ = update_chat_read_inbox.last_read_inbox_message_id_;
                d->chatData[id]->unread_count_ = update_chat_read_inbox.unread_count_;

                Q_EMIT keyDataChanged(to(id), {});
            },
            [this](TDApi::updateChatPermissions &update_chat_permissions) {
                auto id = update_chat_permissions.chat_id_;

                d->chatData[id]->permissions_ = std::move(update_chat_permissions.permissions_) ;

                Q_EMIT keyDataChanged(to(id), {});
            },
            [this](TDApi::updateUserChatAction &update_user_chat_action) {
                auto cid = update_user_chat_action.chat_id_;
                auto uid = update_user_chat_action.user_id_;

                if (update_user_chat_action.action_->get_id() == TDApi::chatActionCancel::ID) {
                    d->ensure(cid).erase(uid);
                }

                d->ensure(cid)[uid] = std::move(update_user_chat_action.action_);
                Q_EMIT keyDataChanged(to(cid), {});
            },
            [](auto& update) { qWarning() << "unhandled chatsmodel update" << QString::fromStdString(TDApi::to_string(update)); }));
}

QVariant ChatsStore::data(const QVariant& key, int role)
{
    if (!checkKey(key)) {
        return QVariant();
    }

    auto chatID = from(key);

    auto r = Roles(role);

    switch (r) {
    case Roles::Title: {
        return QString::fromStdString(d->chatData[chatID]->title_);
    }
    case Roles::Photo: {
        if (d->chatData[chatID]->photo_ == nullptr) {
            return QString();
        }
        return imageToURL(d->chatData[chatID]->photo_->big_);
    }
    case Roles::LastMessageID: {
        if (!d->chatData[chatID]->last_message_) {
            return QString();
        }

        return QString::number(d->chatData[chatID]->last_message_->id_);
    }
    case Roles::UnreadCount: {
        return d->chatData[chatID]->unread_count_;
    }
    case Roles::Kind: {
        const auto id = d->chatData[chatID]->type_->get_id();
        using namespace TDApi;
        const auto map = QMap<std::int32_t, QString>{
            {chatTypeBasicGroup::ID, "basicGroup"},
            {chatTypePrivate::ID,    "privateChat"},
            {chatTypeSecret::ID,     "secretChat"},
            {chatTypeSupergroup::ID, "superGroup"},
        };
        return map[id];
    }
    case Roles::KindID: {
        using namespace TDApi;
        match (d->chatData[chatID]->type_)
            handleCase(chatTypeBasicGroup, basic)
                return QString::number(basic->basic_group_id_);
            endhandle
            handleCase(chatTypeSupergroup, supergroup)
                return QString::number(supergroup->supergroup_id_);
            endhandle
        endmatch
        return QVariant();
    }
    case Roles::IsChannel: {
        using namespace TDApi;
        match (d->chatData[chatID]->type_)
            handleCase(chatTypeSupergroup, supergroup)
                return supergroup->is_channel_;
            endhandle
        endmatch
        return false;
    }
    // permissions
    case Roles::CanSendMessages: return d->chatData[chatID]->permissions_->can_send_messages_;
    case Roles::CanSendMedia: return d->chatData[chatID]->permissions_->can_send_media_messages_;
    case Roles::CanSendPolls: return d->chatData[chatID]->permissions_->can_send_polls_;
    case Roles::CanSendOther: return d->chatData[chatID]->permissions_->can_send_other_messages_;
    case Roles::CanSendWebPreview: return d->chatData[chatID]->permissions_->can_add_web_page_previews_;
    case Roles::CanChangeInfo: return d->chatData[chatID]->permissions_->can_change_info_;
    case Roles::CanInviteUsers: return d->chatData[chatID]->permissions_->can_invite_users_;
    case Roles::CanPinMessages: return d->chatData[chatID]->permissions_->can_pin_messages_;
    case Roles::CurrentActions: {
        return prepare(chatID);
    }
    case Roles::HeaderText: {
        const auto id = d->chatData[chatID]->type_->get_id();
        using namespace TDApi;
        const auto map = QMap<std::int32_t, QString>{
            {chatTypeBasicGroup::ID, i18n("Group")},
            {chatTypePrivate::ID,    i18n("Private chat")},
            {chatTypeSecret::ID,     i18n("Secret chat")},
            {chatTypeSupergroup::ID, i18n("Group")},
        };
        return map[id];
    }
    case Roles::OwnStatus: {
        using namespace TDApi;

        auto pData = PermsData{
            .canRemove = false
        };

        const auto typeID = d->chatData[chatID]->type_->get_id();
        if (typeID != chatTypeBasicGroup::ID && typeID != chatTypeSupergroup::ID) {
            return QVariant::fromValue(pData);
        }

        if (!d->extendedData.contains(chatID)) {
            fetchExtended(chatID);
            return QVariant::fromValue(pData);
        }

        auto& data = d->extendedData[chatID];

        object_ptr<ChatMemberStatus>& status = std::visit([](auto&& arg) -> object_ptr<ChatMemberStatus>& {
            return arg->status_;
        }, data);

        match (status)
            handleCase(chatMemberStatusAdministrator, admin)
                pData.canRemove = admin->can_restrict_members_;
            endhandle
            handleCase(chatMemberStatusCreator, owner)
                Q_UNUSED(owner)

                pData.canRemove = true;
            endhandle
        endmatch

        return QVariant::fromValue(pData);
    }
    }

    return QVariant();
}

bool ChatsStore::checkKey(const QVariant& key)
{
    return d->chatData.contains(from(key));
}

bool ChatsStore::canFetchKey(const QVariant& key)
{
    return true;
}

void ChatsStore::fetchKey(const QVariant& key)
{
    if (from(key) == 0) {
        return;
    }
    c->call<TDApi::getChat>(nullptr, from(key));
}

void ChatsStore::fetchExtended(TDApi::int53 chatID)
{
    using namespace TDApi;

    match (d->chatData[chatID]->type_)
        handleCase(chatTypeBasicGroup, bgroup)
            c->call<TDApi::getBasicGroup>(
                [this, chatID](TDApi::getBasicGroup::ReturnType r) {
                    d->extendedData[chatID] = std::move(r);
                    Q_EMIT keyDataChanged(to(chatID), {});
                },
                bgroup->basic_group_id_
            );
        endhandle
        handleCase(chatTypeSupergroup, sgroup)
            c->call<TDApi::getSupergroup>(
                [this, chatID](TDApi::getSupergroup::ReturnType r) {
                    d->extendedData[chatID] = std::move(r);
                    Q_EMIT keyDataChanged(to(chatID), {});
                },
                sgroup->supergroup_id_
            );
        endhandle
    endmatch
}

QHash<int,QByteArray> ChatsStore::roleNames()
{
    QHash<int,QByteArray> roles;

    roles[int(Roles::Title)] = "mTitle";
    roles[int(Roles::Photo)] = "mPhoto";
    roles[int(Roles::LastMessageID)] = "mLastMessageID";
    roles[int(Roles::UnreadCount)] = "mUnreadCount";
    roles[int(Roles::IsChannel)] = "mIsChannel";
    roles[int(Roles::CanSendMessages)] = "mCanSendMessages";
    roles[int(Roles::CanSendMedia)] = "mCanSendMedia";
    roles[int(Roles::CanSendPolls)] = "mCanSendPolls";
    roles[int(Roles::CanSendOther)] = "mCanSendOther";
    roles[int(Roles::CanSendWebPreview)] = "mCanSendWebPreview";
    roles[int(Roles::CanChangeInfo)] = "mCanChangeInfo";
    roles[int(Roles::CanInviteUsers)] = "mCanInviteUsers";
    roles[int(Roles::CanPinMessages)] = "mCanPinMessages";
    roles[int(Roles::Kind)] = "mKind";
    roles[int(Roles::KindID)] = "mKindID";
    roles[int(Roles::CurrentActions)] = "mCurrentActions";
    roles[int(Roles::HeaderText)] = "mHeaderText";
    roles[int(Roles::OwnStatus)] = "mOwnStatus";

    return roles;
}
