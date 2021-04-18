#include "messagesmodel.h"
#include "chatsstore_p.h"
#include "overloader.h"

enum Roles {
    Title = Qt::UserRole,
    LastMessageID,
    Photo,
    UnreadCount,
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
                qDebug() << "update last message...";

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
    c->call<TDApi::getChat>(nullptr, from(key));
}

QHash<int,QByteArray> ChatsStore::roleNames()
{
    QHash<int,QByteArray> roles;

    roles[int(Roles::Title)] = "mTitle";
    roles[int(Roles::Photo)] = "mPhoto";
    roles[int(Roles::LastMessageID)] = "mLastMessageID";
    roles[int(Roles::UnreadCount)] = "mUnreadCount";

    return roles;
}
