// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QTextBlock>
#include <QQuickTextDocument>

#include "messagesmodel_p.h"
#include "client.h"
#include "chatsstore.h"
#include "chatsstore_p.h"

enum Roles {
    NextID,
    ID,
    ChatID,
    PreviousID,
};

QHash<int,QByteArray> MessagesModel::roleNames() const
{
    QHash<int,QByteArray> roles;

    roles[Roles::NextID] = "mNextID";
    roles[Roles::ID] = "mID";
    roles[Roles::ChatID] = "mChatID";
    roles[Roles::PreviousID] = "mPreviousID";

    return roles;
}

MessagesModel::MessagesModel(Client* parent, TDApi::int53 id) : QAbstractListModel(parent), c(parent), d(new Private)
{
    d->id = id;

    d->messages.push_back(c->chatsStore()->d->chatData[id]->last_read_inbox_message_id_);

    fetch();
}

MessagesModel::~MessagesModel()
{
}

bool MessagesModel::canFetchMore(const QModelIndex& parent) const
{
    return true;
}

void MessagesModel::fetchMore(const QModelIndex& parent)
{
    fetch();
}

void MessagesModel::fetch()
{
    c->call<TDApi::getChatHistory>(
        [=, this](TDApi::getChatHistory::ReturnType resp) {
            bool alreadyHasAll = true;
            for (const auto& msg : resp->messages_) {
                if (std::find(d->messages.cbegin(), d->messages.cend(), msg->id_) == d->messages.cend()) {
                    alreadyHasAll = false;
                    break;
                }
            }

            if (alreadyHasAll) {
                return;
            }

            TDApi::array<TDApi::object_ptr<TDApi::message>> msgs;

            for (auto& msg : resp->messages_) {
                if (std::find(d->messages.cbegin(), d->messages.cend(), msg->id_) == d->messages.cend()) {
                    msgs.push_back(std::move(msg));
                }
            }

            beginInsertRows(QModelIndex(), d->messages.size(), d->messages.size()+msgs.size()-1);
            for (auto& msg : msgs) {
                d->messages.push_back(msg->id_);
                c->messagesStore()->newMessage(std::move(msg));
            }
            endInsertRows();
            dataChanged(index(0), index(0), {Roles::PreviousID, Roles::NextID});
        },
        d->id, d->messages.empty() ? 0 : d->messages[d->messages.size()-1], 0, 50, false
    );
}

void MessagesModel::fetchBack()
{
    if (d->isFetchingBack || !d->canFetchBack) {
        return;
    }

    d->isFetchingBack = true;
    c->call<TDApi::getChatHistory>(
        [=, this](TDApi::getChatHistory::ReturnType resp) {
            d->isFetchingBack = false;

            bool alreadyHasAll = true;
            for (const auto& msg : resp->messages_) {
                if (std::find(d->messages.cbegin(), d->messages.cend(), msg->id_) == d->messages.cend()) {
                    alreadyHasAll = false;
                    break;
                }
            }

            if (alreadyHasAll) {
                d->canFetchBack = false;
                return;
            }

            TDApi::array<TDApi::object_ptr<TDApi::message>> msgs;

            for (auto& msg : resp->messages_) {
                if (std::find(d->messages.cbegin(), d->messages.cend(), msg->id_) == d->messages.cend()) {
                    msgs.push_back(std::move(msg));
                }
            }

            std::reverse(msgs.begin(), msgs.end());

            beginInsertRows(QModelIndex(), 0, msgs.size()-1);
            for (auto& msg : msgs) {
                d->messages.push_front(msg->id_);
                c->messagesStore()->newMessage(std::move(msg));
            }
            endInsertRows();

            auto it = msgs.size();
            dataChanged(index(it), index(it), {Roles::PreviousID, Roles::NextID});
        },
        d->id, d->messages[0], -50, 50, false
    );
}

QVariant MessagesModel::data(const QModelIndex& idx, int role) const
{
    if (!checkIndex(idx, CheckIndexOption::IndexIsValid)) {
        return QVariant();
    }

    // first item in a listview is always loaded for reasons
    if (idx.row() <= 5 && idx.row() > 0) {
        const_cast<MessagesModel*>(this)->fetchBack();
    }

    auto mID = d->messages[idx.row()];

    Roles r = Roles(role);

    switch (r) {

    case Roles::ID: {
        return QString::number(mID);
    }

    case Roles::PreviousID: {
        if (!(int(d->messages.size()) > idx.row()+1)) {
            return QString();
        }

        return QString::number(d->messages[idx.row()+1]);
    }

    case Roles::NextID: {
        if (idx.row() - 1 < 0) {
            return QString();
        }

        return QString::number(d->messages[idx.row()-1]);
    }

    case Roles::ChatID: {
        return QString::number(d->id);
    }

    }

    return QVariant();
}

int MessagesModel::rowCount(const QModelIndex& parent) const
{
    return d->messages.size();
}

void MessagesModel::deletedMessages(const TDApi::array<TDApi::int53>& msgIDs)
{
    for (auto mID : msgIDs) {
        for (auto i = 0UL; i < d->messages.size(); i++) {
            if (d->messages[i] == mID) {
                beginRemoveRows(QModelIndex(), i, i);
                d->messages.erase(d->messages.begin() + i);
                endRemoveRows();
                goto contOuter;
            }
        }
    contOuter:
        ;
    }
}

void MessagesModel::messageIDChanged(TDApi::int53 oldID, TDApi::int53 newID)
{
    auto it = std::find(d->messages.cbegin(), d->messages.cend(), oldID);
    if (it == d->messages.cend()) {
        return;
    }
    auto idx = std::distance(d->messages.cbegin(), it);

    auto before = idx-1;
    auto after = idx+1;

    d->messages[idx] = newID;

    if (before > 0) {
        dataChanged(index(before), index(before), {Roles::PreviousID, Roles::NextID});
    }
    if (after < d->messages.size()) {
        dataChanged(index(after), index(after), {Roles::PreviousID, Roles::NextID});
    }
    dataChanged(index(idx), index(idx), {Roles::ID});
}

void MessagesModel::newMessage(TDApi::int53 msg)
{
    if (d->canFetchBack) return;

    if (std::find(d->messages.cbegin(), d->messages.cend(), msg) != d->messages.cend()) {
        return;
    }

    beginInsertRows(QModelIndex(), 0, 0);
    d->messages.push_front(msg);
    endInsertRows();

    dataChanged(index(1), index(1), {Roles::PreviousID, Roles::NextID});
}

void MessagesModel::messagesInView(QVariantList list)
{
    TDApi::array<TDApi::int53> ids;
    for (auto item : list) {
        ids.push_back(item.toString().toLongLong());
    }

    c->call<TDApi::viewMessages>(
        [](TDApi::viewMessages::ReturnType) {},
        d->id, 0, ids, false
    );
}

static auto format(const QString& txt)
{
    auto text = txt.toStdString();

    auto textParseMarkdown = TDApi::make_object<TDApi::textParseModeMarkdown>( 2 );
    auto parseTextEntities = TDApi::make_object<TDApi::parseTextEntities>( text, std::move( textParseMarkdown ) );

    td::Client::Request parseRequest { 123, std::move( parseTextEntities ) };
    auto parseResponse = td::Client::execute( std::move( parseRequest ) );

    auto formattedText = TDApi::make_object<TDApi::formattedText>();

    if ( parseResponse.object->get_id() == TDApi::formattedText::ID )
    {
        formattedText = TDApi::move_object_as<TDApi::formattedText>( parseResponse.object );
    }
    else
    {
        std::vector<TDApi::object_ptr<TDApi::textEntity>> entities;
        formattedText = TDApi::make_object<TDApi::formattedText>( text, std::move(entities) );
    }

    return formattedText;
}

static auto format(QTextDocument* doku)
{
    auto formattedText = TDApi::make_object<TDApi::formattedText>();
    formattedText->text_ = doku->toPlainText().toStdString();

    for (int block = 0; block < doku->blockCount(); block++) {
        auto bloc = doku->findBlockByNumber(block);
        auto fmts = bloc.textFormats();

        for (const auto& fmt : fmts) {
            if (fmt.format.fontWeight() == QFont::Bold) {
                formattedText->entities_.push_back(TDApi::make_object<TDApi::textEntity>(fmt.start, fmt.length, TDApi::make_object<TDApi::textEntityTypeBold>()));
            } else if (fmt.format.fontItalic()) {
                formattedText->entities_.push_back(TDApi::make_object<TDApi::textEntity>(fmt.start, fmt.length, TDApi::make_object<TDApi::textEntityTypeItalic>()));
            } else if (fmt.format.fontUnderline()) {
                formattedText->entities_.push_back(TDApi::make_object<TDApi::textEntity>(fmt.start, fmt.length, TDApi::make_object<TDApi::textEntityTypeUnderline>()));
            } else if (fmt.format.fontStrikeOut()) {
                formattedText->entities_.push_back(TDApi::make_object<TDApi::textEntity>(fmt.start, fmt.length, TDApi::make_object<TDApi::textEntityTypeStrikethrough>()));
            } else if (fmt.format.font() == QFontDatabase::systemFont(QFontDatabase::FixedFont)) {
                formattedText->entities_.push_back(TDApi::make_object<TDApi::textEntity>(fmt.start, fmt.length, TDApi::make_object<TDApi::textEntityTypeCode>()));
            }
        }
    }

    return formattedText;
}

static auto isUserFormatted(QTextDocument* doku)
{
    for (int block = 0; block < doku->blockCount(); block++) {
        auto bloc = doku->findBlockByNumber(block);
        auto fmts = bloc.textFormats();

        for (const auto& fmt : fmts) {
            if (fmt.format.fontWeight() == QFont::Bold) {
                return true;
            } else if (fmt.format.fontItalic()) {
                return true;
            } else if (fmt.format.fontUnderline()) {
                return true;
            } else if (fmt.format.fontStrikeOut()) {
                return true;
            } else if (fmt.format.font() == QFontDatabase::systemFont(QFontDatabase::FixedFont)) {
                return true;
            }
        }
    }

    return false;
}

static auto format(QQuickTextDocument* doku)
{
    auto doc = doku->textDocument();

    if (isUserFormatted(doc)) {
        return format(doc);
    }

    return format(doc->toPlainText());
}

static TDApi::object_ptr<TDApi::InputFile> as_local_file(const QUrl& path) {
    return TDApi::make_object<TDApi::inputFileLocal>(path.toLocalFile().toStdString());
}

void MessagesModel::send(SendData data)
{
    auto send_message = TDApi::make_object<TDApi::sendMessage>();
    send_message->chat_id_ = d->id;

    if (data.replyToID != 0) {
        send_message->reply_to_message_id_ = data.replyToID;
    }

    if (auto it = std::get_if<SendData::Text>(&data.contents)) {
        auto message_content = TDApi::make_object<TDApi::inputMessageText>();
        message_content->text_ = std::move(it->s);

        send_message->input_message_content_ = std::move(message_content);
    } else if (auto it = std::get_if<SendData::Photo>(&data.contents)) {
        auto message_content = TDApi::make_object<TDApi::inputMessagePhoto>();
        message_content->photo_ = as_local_file(it->p);
        message_content->caption_ = std::move(it->s);

        send_message->input_message_content_ = std::move(message_content);
    } else if (auto it = std::get_if<SendData::File>(&data.contents)) {
        auto message_content = TDApi::make_object<TDApi::inputMessageDocument>();
        message_content->document_ = as_local_file(it->p);
        message_content->caption_ = std::move(it->s);
        message_content->disable_content_type_detection_ = true;

        send_message->input_message_content_ = std::move(message_content);
    } else if (auto it = std::get_if<SendData::Video>(&data.contents)) {
        auto message_content = TDApi::make_object<TDApi::inputMessageVideo>();
        message_content->video_ = as_local_file(it->p);
        message_content->caption_ = std::move(it->s);

        send_message->input_message_content_ = std::move(message_content);
    } else if (auto it = std::get_if<SendData::Audio>(&data.contents)) {
        auto message_content = TDApi::make_object<TDApi::inputMessageAudio>();
        message_content->audio_ = as_local_file(it->p);
        message_content->caption_ = std::move(it->s);

        send_message->input_message_content_ = std::move(message_content);
    }

    c->callP<TDApi::sendMessage>(
        [=](TDApi::sendMessage::ReturnType) {},
        std::move(send_message)
    );
}

void MessagesModel::send(QQuickTextDocument* doku, const QString& inReplyTo)
{
    send(SendData {
        .contents = SendData::Text {format(doku)},
        .replyToID = inReplyTo.toLongLong()
    });
}

void MessagesModel::sendAttachment(QQuickTextDocument* doku, QUrl url, const QString& kind)
{
    if (kind == "video") {
        send(SendData {
            .contents = SendData::Video {format(doku), url},
        });
    } else if (kind == "image") {
        send(SendData {
            .contents = SendData::Photo {format(doku), url},
        });
    } else if (kind == "audio") {
        send(SendData {
            .contents = SendData::Audio {format(doku), url},
        });
    } else if (kind == "file") {
        send(SendData {
            .contents = SendData::File {format(doku), url},
        });
    } else {
        Q_UNREACHABLE();
    }
}

void MessagesModel::sendFile(QQuickTextDocument* doku, QUrl url, const QString& inReplyTo)
{
    send(SendData {
        .contents = SendData::File {format(doku), url},
        .replyToID = inReplyTo.toLongLong()
    });
}

void MessagesModel::sendPhoto(QQuickTextDocument* doku, QUrl url, const QString& inReplyTo)
{
    send(SendData {
        .contents = SendData::Photo {format(doku), url},
        .replyToID = inReplyTo.toLongLong()
    });
}

void MessagesModel::edit(QQuickTextDocument* doku, const QString& messageID)
{
    auto edit_message = TDApi::make_object<TDApi::editMessageText>();
    edit_message->chat_id_ = d->id;
    edit_message->message_id_ = messageID.toLongLong();
    edit_message->input_message_content_ = TDApi::make_object<TDApi::inputMessageText>(format(doku), false, false);

    c->callP<TDApi::editMessageText>(nullptr, std::move(edit_message));
}

void MessagesModel::comingIn()
{
    c->call<TDApi::openChat>([](TDApi::openChat::ReturnType) {}, d->id);
}

void MessagesModel::comingOut()
{
    c->call<TDApi::closeChat>([](TDApi::openChat::ReturnType) {}, d->id);
}

QIviPendingReplyBase MessagesModel::hopBackToMessage(const QString& id)
{
    QIviPendingReply<int> ret;

    auto mid = id.toLongLong();

    auto idx = std::find(d->messages.cbegin(), d->messages.cend(), mid);
    if (idx != d->messages.cend()) {
        auto msg = idx - d->messages.cbegin();
        ret.setSuccess(msg);
        return ret;
    }

    c->call<TDApi::getChatHistory>(
        [mid, ret, this](TDApi::getChatHistory::ReturnType resp) mutable {
            const auto oldSize = d->messages.size();

            beginInsertRows(QModelIndex(), d->messages.size(), d->messages.size()+resp->messages_.size()-1);
            for (auto& msg : resp->messages_) {
                d->messages.push_back(msg->id_);
                c->messagesStore()->newMessage(std::move(msg));
            }
            endInsertRows();
            dataChanged(index(0), index(0), {Roles::PreviousID, Roles::NextID});

            auto idx = std::find(d->messages.cbegin(), d->messages.cend(), mid);
            auto msg = idx - d->messages.cbegin();
            ret.setSuccess(msg);

            QTimer::singleShot(110, [oldSize, this]() {
                beginRemoveRows(QModelIndex(), 0, oldSize-1);

                int i = oldSize-1;
                quint32 itemsRemoved = 0;
                while (i >= 0) {
                    i--;
                    itemsRemoved++;
                    d->messages.pop_front();
                }

                d->canFetchBack = true;

                Q_ASSERT(itemsRemoved == oldSize);

                endRemoveRows();
            });
        },
        d->id, mid, -25, 50, false
    );

    return ret;
}
