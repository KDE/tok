#include "chatsmodel.h"
#include "chatsmodel_p.h"

#include "chatsstore.h"

#include "messagesmodel.h"

#include "overloader.h"

#include <KLocalizedString>

#include <td/telegram/td_api.h>
#include <td/tl/TlObject.h>

enum Roles {
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
        [](TDApi::getChats::ReturnType resp) {
        },
        nullptr, std::numeric_limits<std::int64_t>::max(), 0, 20
    );
}

void ChatsModel::newChat(TDApi::object_ptr<TDApi::chat> ch)
{
    beginInsertRows(QModelIndex(), d->chats.size(), d->chats.size());
    d->chats.push_back(ch->id_);
    endInsertRows();

    c->chatsStore()->newChat(std::move(ch));
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
            [this, &u](TDApi::updateChatTitle &update_chat_title) {
                c->chatsStore()->handleUpdate(std::move(u));
            },
            [this, &u](TDApi::updateChatLastMessage &update_chat_last_message) {
                c->chatsStore()->handleUpdate(std::move(u));
            },
            [this, &u](TDApi::updateChatReadInbox &update_chat_read_inbox) {
                c->chatsStore()->handleUpdate(std::move(u));
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

    roles[int(Roles::ID)] = "mID";

    return roles;
}

QVariant ChatsModel::data(const QModelIndex& idx, int role) const
{
    if (!checkIndex(idx, CheckIndexOption::IndexIsValid)) {
        return QVariant();
    }

    auto chatID = d->chats[idx.row()];
    auto r = Roles(role);

    switch (r) {
    case Roles::ID: {
        return QString::number(chatID);
    }
    }

    return QVariant();
}