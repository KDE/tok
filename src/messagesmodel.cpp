#include "messagesmodel_p.h"
#include "client.h"

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
            beginInsertRows(QModelIndex(), d->messages.size(), d->messages.size()+resp->messages_.size()-1);
            for (auto& msg : resp->messages_) {
                if (std::find(d->messages.cbegin(), d->messages.cend(), msg->id_) != d->messages.cend()) {
                    continue;
                }

                d->messages.push_back(msg->id_);
                c->messagesStore()->newMessage(std::move(msg));
            }
            endInsertRows();
            dataChanged(index(0), index(0), {Roles::PreviousID, Roles::NextID});
        },
        d->id, d->messages.empty() ? 0 : d->messages[d->messages.size()-1], 0, 50, false
    );
}

QVariant MessagesModel::data(const QModelIndex& idx, int role) const
{
    if (!checkIndex(idx, CheckIndexOption::IndexIsValid)) {
        return QVariant();
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
        message_content->text_ = format(it->s);

        send_message->input_message_content_ = std::move(message_content);
    } else if (auto it = std::get_if<SendData::Photo>(&data.contents)) {
        auto message_content = TDApi::make_object<TDApi::inputMessagePhoto>();
        message_content->photo_ = as_local_file(it->p);
        message_content->caption_ = format(it->s);

        send_message->input_message_content_ = std::move(message_content);
    } else if (auto it = std::get_if<SendData::File>(&data.contents)) {
        auto message_content = TDApi::make_object<TDApi::inputMessageDocument>();
        message_content->document_ = as_local_file(it->p);
        message_content->caption_ = format(it->s);
        message_content->disable_content_type_detection_ = true;

        send_message->input_message_content_ = std::move(message_content);
    }

    c->callP<TDApi::sendMessage>(
        [=](TDApi::sendMessage::ReturnType) {},
        std::move(send_message)
    );
}

void MessagesModel::send(const QString& contents, const QString& inReplyTo)
{
    send(SendData {
        .contents = SendData::Text {contents},
        .replyToID = inReplyTo.toLongLong()
    });
}

void MessagesModel::sendFile(const QString& contents, QUrl url, const QString& inReplyTo)
{
    send(SendData {
        .contents = SendData::File {contents, url},
        .replyToID = inReplyTo.toLongLong()
    });
}

void MessagesModel::sendPhoto(const QString& contents, QUrl url, const QString& inReplyTo)
{
    send(SendData {
        .contents = SendData::Photo {contents, url},
        .replyToID = inReplyTo.toLongLong()
    });
}

void MessagesModel::comingIn()
{
    c->call<TDApi::openChat>([](TDApi::openChat::ReturnType) {}, d->id);
}

void MessagesModel::comingOut()
{
    c->call<TDApi::closeChat>([](TDApi::openChat::ReturnType) {}, d->id);
}
