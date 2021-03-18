#include <QJSValue>

#include "messagesmodel_p.h"

enum Roles {
    AuthorID = Qt::UserRole,
    Kind,
    Timestamp,

    // Text messages
    Content,

    // Photo messages
    ImageURL,
    ImageCaption,
};

MessagesStore::MessagesStore(Client* parent) : c(parent), d(new Private)
{

}

MessagesStore::~MessagesStore()
{

}

std::pair<TDApi::int53, TDApi::int53> fromVariant(const QVariant& hi)
{
    const auto js = hi.value<QJSValue>();
    return std::make_pair(js.property(0).toString().toLongLong(), js.property(1).toString().toLongLong());
}

void MessagesStore::newMessage(TDApi::object_ptr<TDApi::message> msg)
{
    QJsonArray mu;
    mu << QString::number(msg->chat_id_) << QString::number(msg->id_);
    d->messageData[std::make_pair(msg->chat_id_, msg->id_)] = std::move(msg);
    Q_EMIT keyAdded(mu);
}

QVariant MessagesStore::data(const QVariant& key, int role)
{
    if (!checkKey(key)) {
        return QVariant();
    }

    auto mID = fromVariant(key);

    switch (Roles(role)) {
    case Roles::Timestamp: {
        return QDateTime::fromTime_t(d->messageData[mID]->date_).toString("hh:mm");
    }
    case Roles::Content: {
        auto content = d->messageData[mID]->content_.get();
        if (content->get_id() != TDApi::messageText::ID) {
            return QString();
        }

        return QString::fromStdString(static_cast<TDApi::messageText*>(content)->text_->text_);
    }
    case Roles::ImageURL: {
        auto content = d->messageData[mID]->content_.get();
        if (content->get_id() != TDApi::messagePhoto::ID) {
            return QString();
        }
        auto image = static_cast<TDApi::messagePhoto*>(content);
        int sz = 0;
        int trueI = -1;
        int i = 0;
        for (auto& size : image->photo_->sizes_) {
            auto thisSz = size->height_ * size->width_;
            if (sz < thisSz) {
                sz = thisSz;
                trueI = i;
            }
            i++;
        }
        return QString("image://telegram/%1").arg(image->photo_->sizes_[trueI]->photo_->id_);
    }
    case Roles::ImageCaption: {
        auto content = d->messageData[mID]->content_.get();
        if (content->get_id() != TDApi::messagePhoto::ID) {
            return QString();
        }

        auto image = static_cast<TDApi::messagePhoto*>(content);
        if (image->caption_ == nullptr) {
            return QString();
        }
        return QString::fromStdString(image->caption_->text_);
    }

    case Roles::AuthorID: {
        const auto idFrom = [](TDApi::MessageSender* s) {
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
        return idFrom(d->messageData[mID]->sender_.get());
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

    Q_UNREACHABLE();
}

bool MessagesStore::checkKey(const QVariant& key)
{
    return d->messageData.contains(fromVariant(key));
}

bool MessagesStore::canFetchKey(const QVariant& key)
{
    return true;
}

void MessagesStore::fetchKey(const QVariant& key)
{
    auto [chat, msg] = fromVariant(key);
    c->call<TDApi::getMessage>(
        [](TDApi::getMessage::ReturnType) {},
        chat, msg
    );
}


QHash<int, QByteArray> MessagesStore::roleNames()
{
    QHash<int,QByteArray> roles;

    roles[Roles::AuthorID] = "authorID";
    roles[Roles::Kind] = "kind";
    roles[Roles::Timestamp] = "timestamp";

    roles[Roles::Content] = "content";

    roles[Roles::ImageURL] = "imageURL";
    roles[Roles::ImageCaption] = "imageCaption";

    return roles;
}
