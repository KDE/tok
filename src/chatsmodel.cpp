#include "chatsmodel.h"
#include "chatsmodel_p.h"

#include "overloader.h"

#include <KLocalizedString>

#include <td/telegram/td_api.h>
#include <td/tl/TlObject.h>

enum Roles {
    Title = Qt::UserRole,
    Subtitle,
    Photo,
    ID,
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
            // beginInsertRows(QModelIndex(), d->chats.size(), d->chats.size() + resp->chat_ids_.size());
            for (auto chat : resp->chat_ids_) {
                d->chats.push_back(chat);
            }
            endResetModel();
            // endInsertRows();

            if (resp->chat_ids_.size() == 0) {
                d->atEnd = true;
            }
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
                updateChat(std::move(update_new_chat.chat_));
            },
            [this](TDApi::updateChatTitle &update_chat_title) {
                auto id = update_chat_title.chat_id_;

                if (d->chatData.contains(id)) {
                    d->chatData[id]->title_ = update_chat_title.title_;

                    auto v = d->locateChatIndex(id);
                    if (!v.has_value()) {
                        return;
                    }

                    Q_EMIT dataChanged(index(*v), index(*v), {Roles::Subtitle});
                }
            },
            [this](TDApi::updateChatLastMessage &update_chat_last_message) {
                auto id = update_chat_last_message.chat_id_;

                if (d->chatData.contains(id)) {
                    d->chatData[id]->last_message_ = std::move(update_chat_last_message.last_message_);

                    auto v = d->locateChatIndex(id);
                    if (!v.has_value()) {
                        return;
                    }

                    Q_EMIT dataChanged(index(*v), index(*v), {Roles::Subtitle});
                }
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
    roles[int(Roles::Subtitle)] = "mSubtitle";
    roles[int(Roles::ID)] = "mID";

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
        // TODO: photo
        return QVariant();
    }
    case Roles::Subtitle: {
        if (!d->chatData[chatID]->last_message_) {
            return QString();
        }

        auto prefix = [=, this]() {
            switch (d->chatData[chatID]->last_message_->sender_->get_id()) {
            case TDApi::messageSenderUser::ID: {
                auto moved = static_cast<TDApi::messageSenderUser*>(d->chatData[chatID]->last_message_->sender_.get());
                return QString::number(moved->user_id_) + QString(": ");
            }
            case TDApi::messageSenderChat::ID: {
                return QString();
            }
            }

            qWarning() << "unhandled author:" << QString::fromStdString(TDApi::to_string(d->chatData[chatID]->last_message_->sender_));

            return QString("unsupported");
        };

        switch (d->chatData[chatID]->last_message_->content_->get_id()) {
        case TDApi::messageText::ID: {
            auto moved = static_cast<TDApi::messageText*>(d->chatData[chatID]->last_message_->content_.get());
            return prefix() + QString::fromStdString(moved->text_->text_);
        }
        case TDApi::messageChatAddMembers::ID: {
            auto moved = static_cast<TDApi::messageChatAddMembers*>(d->chatData[chatID]->last_message_->content_.get());

            QStringList its;
            for (auto item : moved->member_user_ids_) {
                its << QString::number(item);
            }

            return i18np("%2 joined the chat", "%2 joined the chat", moved->member_user_ids_.size(), its.join(", "));
        }
        }

        qWarning() << "unhandled content:" << QString::fromStdString(TDApi::to_string(d->chatData[chatID]->last_message_->content_));

        return QString("unsupported");
    }
    }

    return QVariant();
}