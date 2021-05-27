#include "messagesmodel.h"
#include "chatsstore_p.h"
#include "overloader.h"

enum Roles {
    Title = Qt::UserRole,
    LastMessageID,
    Photo,
    UnreadCount,
    Kind,
    CanSendMessages,
    CanSendMedia, // True, if the user can send audio files, documents, photos, videos, video notes, and voice notes. Implies can_send_messages permissions.
    CanSendPolls,
    CanSendOther, // True, if the user can send animations, games, stickers, and dice and use inline bots. Implies can_send_messages permissions.
    CanSendWebPreview,
    CanChangeInfo,
    CanInviteUsers,
    CanPinMessages,
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
        return QString("image://telegram/%1").arg(d->chatData[chatID]->photo_->big_->id_);
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
    // permissions
    case Roles::CanSendMessages: return d->chatData[chatID]->permissions_->can_send_messages_;
    case Roles::CanSendMedia: return d->chatData[chatID]->permissions_->can_send_media_messages_;
    case Roles::CanSendPolls: return d->chatData[chatID]->permissions_->can_send_polls_;
    case Roles::CanSendOther: return d->chatData[chatID]->permissions_->can_send_other_messages_;
    case Roles::CanSendWebPreview: return d->chatData[chatID]->permissions_->can_add_web_page_previews_;
    case Roles::CanChangeInfo: return d->chatData[chatID]->permissions_->can_change_info_;
    case Roles::CanInviteUsers: return d->chatData[chatID]->permissions_->can_invite_users_;
    case Roles::CanPinMessages: return d->chatData[chatID]->permissions_->can_pin_messages_;
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

QHash<int,QByteArray> ChatsStore::roleNames()
{
    QHash<int,QByteArray> roles;

    roles[int(Roles::Title)] = "mTitle";
    roles[int(Roles::Photo)] = "mPhoto";
    roles[int(Roles::LastMessageID)] = "mLastMessageID";
    roles[int(Roles::UnreadCount)] = "mUnreadCount";
    roles[int(Roles::CanSendMessages)] = "mCanSendMessages";
    roles[int(Roles::CanSendMedia)] = "mCanSendMedia";
    roles[int(Roles::CanSendPolls)] = "mCanSendPolls";
    roles[int(Roles::CanSendOther)] = "mCanSendOther";
    roles[int(Roles::CanSendWebPreview)] = "mCanSendWebPreview";
    roles[int(Roles::CanChangeInfo)] = "mCanChangeInfo";
    roles[int(Roles::CanInviteUsers)] = "mCanInviteUsers";
    roles[int(Roles::CanPinMessages)] = "mCanPinMessages";
    roles[int(Roles::Kind)] = "mKind";

    return roles;
}
