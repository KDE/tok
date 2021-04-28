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

void MessagesModel::send(const QString& contents)
{
    auto send_message = TDApi::make_object<TDApi::sendMessage>();
    send_message->chat_id_ = d->id;
    auto message_content = TDApi::make_object<TDApi::inputMessageText>();
    message_content->text_ = format(contents);
    send_message->input_message_content_ = std::move(message_content);

    c->callP<TDApi::sendMessage>(
        [=](TDApi::sendMessage::ReturnType t) {

        },
        std::move(send_message)
    );
}

void MessagesModel::comingIn()
{
    c->call<TDApi::openChat>([](TDApi::openChat::ReturnType) {}, d->id);
}

void MessagesModel::comingOut()
{
    c->call<TDApi::closeChat>([](TDApi::openChat::ReturnType) {}, d->id);
}
