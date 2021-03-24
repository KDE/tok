#include "chatsmodel.h"
#include "chatsmodel_p.h"

#include "messagesmodel.h"

#include "overloader.h"

#include <KLocalizedString>

#include <td/telegram/td_api.h>
#include <td/tl/TlObject.h>

enum Roles {
    Title = Qt::UserRole,
    LastMessageID,
    Photo,
    ID,
    UnreadCount,
};

ChatsModel::ChatsModel(Client* parent) : QAbstractListModel(parent), c(parent), d(new Private)
{
}

ChatsModel::~ChatsModel()
{
}

void ChatsModel::fetch()
{
    c->call<TDApi::getChats>(
        [](TDApi::getChats::ReturnType resp) {
        },
        nullptr, std::numeric_limits<std::int64_t>::max(), 0, 20
    );
}

void ChatsModel::updateChat(TDApi::object_ptr<TDApi::chat> c)
{
    auto id = c->id_;

    d->chatData[c->id_] = std::move(c);

    auto v = std::find(d->chats.begin(), d->chats.end(), id);
    Q_ASSERT(v != d->chats.end());

    auto idx = *v;

    Q_EMIT dataChanged(index(idx), index(idx), {});
}

void ChatsModel::newChat(TDApi::object_ptr<TDApi::chat> c)
{
    auto id = c->id_;

    d->chatData[c->id_] = std::move(c);

    beginInsertRows(QModelIndex(), d->chats.size(), d->chats.size());
    d->chats.push_back(id);
    endInsertRows();
}

bool ChatsModel::canFetchMore(const QModelIndex& parent) const
{
    return !d->atEnd;
}

void ChatsModel::fetchMore(const QModelIndex& parent)
{
    fetch();
}

void ChatsModel::handleUpdate(TDApi::object_ptr<TDApi::Update> u)
{
    TDApi::downcast_call(*u,
        overloaded(
            [this](TDApi::updateNewChat &update_new_chat) {
                newChat(std::move(update_new_chat.chat_));
            },
            [this](TDApi::updateChatTitle &update_chat_title) {
                auto id = update_chat_title.chat_id_;

                d->chatData[id]->title_ = update_chat_title.title_;

                auto v = d->locateChatIndex(id);
                Q_EMIT dataChanged(index(v), index(v), {});
            },
            [this](TDApi::updateChatLastMessage &update_chat_last_message) {
                if (!update_chat_last_message.last_message_.get()) {
                    return;
                }

                auto id = update_chat_last_message.chat_id_;

                d->chatData[id]->last_message_ = TD::make_tl_object<TDApi::message>();
                d->chatData[id]->last_message_->id_ = update_chat_last_message.last_message_->id_;
                c->messagesStore()->newMessage(std::move(update_chat_last_message.last_message_));

                auto v = d->locateChatIndex(id);
                Q_EMIT dataChanged(index(v), index(v), {});
            },
            [this](TDApi::updateChatReadInbox &update_chat_read_inbox) {
                auto id = update_chat_read_inbox.chat_id_;

                d->chatData[id]->last_read_inbox_message_id_ = update_chat_read_inbox.last_read_inbox_message_id_;
                d->chatData[id]->unread_count_ = update_chat_read_inbox.unread_count_;

                auto v = d->locateChatIndex(id);
                Q_EMIT dataChanged(index(v), index(v), {});
            },
            [](auto& update) { qWarning() << "unhandled chatsmodel update" << QString::fromStdString(TDApi::to_string(update)); }));
}

int ChatsModel::rowCount(const QModelIndex& parent) const
{
    return d->chats.size();
}

QHash<int,QByteArray> ChatsModel::roleNames() const
{
    QHash<int,QByteArray> roles;

    roles[int(Roles::Title)] = "mTitle";
    roles[int(Roles::Photo)] = "mPhoto";
    roles[int(Roles::LastMessageID)] = "mLastMessageID";
    roles[int(Roles::ID)] = "mID";
    roles[int(Roles::UnreadCount)] = "mUnreadCount";

    return roles;
}

QVariant ChatsModel::data(const QModelIndex& idx, int role) const
{
    if (!checkIndex(idx, CheckIndexOption::IndexIsValid)) {
        return QVariant();
    }

    auto chatID = d->chats[idx.row()];
    if (!d->chatData.contains(chatID)) {
        return QVariant();
    }

    auto r = Roles(role);

    switch (r) {
    case Roles::ID: {
        return QString::number(d->chatData[chatID]->id_);
    }
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
    }

    return QVariant();
}