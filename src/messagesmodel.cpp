#include "messagesmodel_p.h"
#include "src/client.h"

enum Roles {
    Content = Qt::UserRole,
    AuthorID,
    PreviousAuthorID,
    NextAuthorID,
    ID,
    Kind,
};

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
                d->messages.push_back(msg->id_);
                d->messageData[msg->id_] = std::move(msg);
            }
            endInsertRows();
            dataChanged(index(0), index(0), {Roles::PreviousAuthorID, Roles::NextAuthorID});
        },
        d->id, d->messages.empty() ? 0 : d->messageData[d->messages[d->messages.size()-1]]->id_, 0, 50, false
    );
}

QVariant MessagesModel::data(const QModelIndex& idx, int role) const
{
    if (!checkIndex(idx, CheckIndexOption::IndexIsValid)) {
        return QVariant();
    }

    auto mID = d->messages[idx.row()];
    if (!d->messageData.contains(mID)) {
        return QVariant();
    }

    Roles r = Roles(role);

    auto idFrom = [](TDApi::MessageSender* s) {
        switch (s->get_id()) {
        case TDApi::messageSenderUser::ID: {
            auto moved = static_cast<TDApi::messageSenderUser*>(s);
            return QString::number(moved->user_id_);
        }
        case TDApi::messageSenderChat::ID: {
            return QString();
        }
        }

        return QString();
    };

    switch (r) {
    case Roles::ID: {
        return QString::number(mID);
    }
    case Roles::Content: {
        switch (d->messageData[mID]->content_->get_id()) {
        case TDApi::messageText::ID: {
            auto moved = static_cast<TDApi::messageText*>(d->messageData[mID]->content_.get());
            return QString::fromStdString(moved->text_->text_);
        }
        }
    }

    case Roles::AuthorID: {
        return idFrom(d->messageData[mID]->sender_.get());
    }

    case Roles::PreviousAuthorID: {
        if (!(int(d->messages.size()) > idx.row()+1)) {
            return QString();
        }

        auto mPrevID = d->messages[idx.row()+1];
        if (!d->messageData.contains(mPrevID)) {
            return QString();
        }

        return idFrom(d->messageData[mPrevID]->sender_.get());
    }

    case Roles::NextAuthorID: {
        if (idx.row() - 1 < 0) {
            return QString();
        }

        auto mNextID = d->messages[idx.row()-1];
        if (!d->messageData.contains(mNextID)) {
            return QString();
        }

        return idFrom(d->messageData[mNextID]->sender_.get());
    }

    case Roles::Kind: {
        switch (d->messageData[mID]->content_->get_id()) {
        case TDApi::messageText::ID: return QString("messageText");
        case TDApi::messageAnimation::ID: return QString("messageAnimation");
        case TDApi::messageAudio::ID: return QString("messageAudio");
        case TDApi::messageDocument::ID: return QString("messageDocument");
        case TDApi::messagePhoto::ID: return QString("messagePhoto");
        case TDApi::messageExpiredPhoto::ID: return QString("messageExpiredPhoto");
        case TDApi::messageSticker::ID: return QString("messageSticker");
        case TDApi::messageVideo::ID: return QString("messageVideo");
        case TDApi::messageExpiredVideo::ID: return QString("messageExpiredVideo");
        case TDApi::messageVideoNote::ID: return QString("messageVideoNote");
        case TDApi::messageVoiceNote::ID: return QString("messageVoiceNote");
        case TDApi::messageLocation::ID: return QString("messageLocation");
        case TDApi::messageVenue::ID: return QString("messageVenue");
        case TDApi::messageContact::ID: return QString("messageContact");
        case TDApi::messageDice::ID: return QString("messageDice");
        case TDApi::messageGame::ID: return QString("messageGame");
        case TDApi::messagePoll::ID: return QString("messagePoll");
        case TDApi::messageInvoice::ID: return QString("messageInvoice");
        case TDApi::messageCall::ID: return QString("messageCall");
        case TDApi::messageVoiceChatStarted::ID: return QString("messageVoiceChatStarted");
        case TDApi::messageVoiceChatEnded::ID: return QString("messageVoiceChatEnded");
        case TDApi::messageInviteVoiceChatParticipants::ID: return QString("messageInviteVoiceChatParticipants");
        case TDApi::messageBasicGroupChatCreate::ID: return QString("messageBasicGroupChatCreate");
        case TDApi::messageSupergroupChatCreate::ID: return QString("messageSupergroupChatCreate");
        case TDApi::messageChatChangeTitle::ID: return QString("messageChatChangeTitle");
        case TDApi::messageChatChangePhoto::ID: return QString("messageChatChangePhoto");
        case TDApi::messageChatDeletePhoto::ID: return QString("messageChatDeletePhoto");
        case TDApi::messageChatAddMembers::ID: return QString("messageChatAddMembers");
        case TDApi::messageChatJoinByLink::ID: return QString("messageChatJoinByLink");
        case TDApi::messageChatDeleteMember::ID: return QString("messageChatDeleteMember");
        case TDApi::messageChatUpgradeTo::ID: return QString("messageChatUpgradeTo");
        case TDApi::messageChatUpgradeFrom::ID: return QString("messageChatUpgradeFrom");
        case TDApi::messagePinMessage::ID: return QString("messagePinMessage");
        case TDApi::messageScreenshotTaken::ID: return QString("messageScreenshotTaken");
        case TDApi::messageChatSetTtl::ID: return QString("messageChatSetTtl");
        case TDApi::messageCustomServiceAction::ID: return QString("messageCustomServiceAction");
        case TDApi::messageGameScore::ID: return QString("messageGameScore");
        case TDApi::messagePaymentSuccessful::ID: return QString("messagePaymentSuccessful");
        case TDApi::messageContactRegistered::ID: return QString("messageContactRegistered");
        case TDApi::messageWebsiteConnected::ID: return QString("messageWebsiteConnected");
        case TDApi::messagePassportDataSent::ID: return QString("messagePassportDataSent");
        case TDApi::messageProximityAlertTriggered::ID: return QString("messageProximityAlertTriggered");
        case TDApi::messageUnsupported::ID: return QString("messageUnsupported");
        }
    }

    }

    return QVariant();
}

int MessagesModel::rowCount(const QModelIndex& parent) const
{
    return d->messages.size();
}

QHash<int,QByteArray> MessagesModel::roleNames() const
{
    QHash<int,QByteArray> roles;

    roles[Roles::Content] = "mContent";
    roles[Roles::AuthorID] = "mAuthorID";
    roles[Roles::PreviousAuthorID] = "mPreviousAuthorID";
    roles[Roles::NextAuthorID] = "mNextAuthorID";
    roles[Roles::ID] = "mID";
    roles[Roles::Kind] = "mKind";

    return roles;
}

void MessagesModel::newMessage(TDApi::object_ptr<TDApi::message> msg)
{
    beginInsertRows(QModelIndex(), 0, 0);
    d->messages.push_front(msg->id_);
    d->messageData[msg->id_] = std::move(msg);
    endInsertRows();

    dataChanged(index(1), index(1), {Roles::PreviousAuthorID, Roles::NextAuthorID});
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

void MessagesModel::send(const QString& contents)
{
    auto send_message = TDApi::make_object<TDApi::sendMessage>();
    send_message->chat_id_ = d->id;
    auto message_content = TDApi::make_object<TDApi::inputMessageText>();
    message_content->text_ = TDApi::make_object<TDApi::formattedText>();
    message_content->text_->text_ = contents.toStdString();
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
