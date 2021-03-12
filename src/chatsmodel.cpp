#include "chatsmodel.h"
#include "chatsmodel_p.h"

#include "overloader.h"
#include <td/telegram/td_api.h>

enum class Roles {
    Title = Qt::UserRole,
    Photo,
    LastMessageAuthorName,
    LastMessageContent,
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
        [=, this](TDApi::getChats::ReturnType resp) {
            beginResetModel();
            for (auto chat : resp->chat_ids_) {
                d->chats.push_back(chat);
            }
            endResetModel();
        },
        nullptr, std::numeric_limits<std::int64_t>::max(), 0, 20
    );
}

void ChatsModel::updateChat(TDApi::object_ptr<TDApi::chat> c)
{
    auto id = c->id_;

    d->chatData[c->id_] = std::move(c);

    auto v = std::find(d->chats.begin(), d->chats.end(), id);
    if (v == d->chats.end()) {
        return;
    }

    auto idx = *v;
    Q_EMIT dataChanged(index(idx), index(idx), {});
}

void ChatsModel::handleUpdate(TDApi::object_ptr<TDApi::Update> u)
{
    TDApi::downcast_call(*u,
        overloaded(
            [this](TDApi::updateNewChat &update_new_chat) {
                updateChat(std::move(update_new_chat.chat_));
            },
            [this](TDApi::updateChatTitle &update_chat_title) {
                if (d->chatData.contains(update_chat_title.chat_id_)) {
                    d->chatData[update_chat_title.chat_id_]->title_ = update_chat_title.title_;
                }
            },
            [this](TDApi::updateChatLastMessage &update_chat_last_message) {
                if (d->chatData.contains(update_chat_last_message.chat_id_)) {
                    d->chatData[update_chat_last_message.chat_id_]->last_message_ = std::move(update_chat_last_message.last_message_);
                }
            },
            [](auto& update) { /* fallback */ }));
}

int ChatsModel::rowCount(const QModelIndex& parent) const
{
    return d->chats.size();
}

QHash<int,QByteArray> ChatsModel::roleNames() const
{
    QHash<int,QByteArray> roles;

    roles[int(Roles::Title)] = "title";
    roles[int(Roles::Photo)] = "photo";
    roles[int(Roles::LastMessageContent)] = "lastMessageContent";
    roles[int(Roles::LastMessageAuthorName)] = "lastMessageAuthorName";

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
    case Roles::Title: {
        return QString::fromStdString(d->chatData[chatID]->title_);
    }
    case Roles::Photo: {
        // TODO: photo
        return QVariant();
    }
    case Roles::LastMessageAuthorName: {
        if (!d->chatData[chatID]->last_message_) {
            return QString();
        }
        switch (d->chatData[chatID]->last_message_->sender_->get_id()) {
        case TDApi::messageSenderUser::ID: {
            auto moved = static_cast<TDApi::messageSenderUser*>(d->chatData[chatID]->last_message_->sender_.get());
            return QString::number(moved->user_id_);
        }
        }

        return QString("unsupported");
    }
    case Roles::LastMessageContent: {
        if (!d->chatData[chatID]->last_message_) {
            return QString();
        }
        switch (d->chatData[chatID]->last_message_->get_id()) {
        case TDApi::messageText::ID: {
            auto moved = static_cast<TDApi::messageText*>(d->chatData[chatID]->last_message_->content_.get());
            return QString::fromStdString(moved->text_->text_);
        }
        }

        return QString("unsupported");
    }
    }

    return QVariant();
}