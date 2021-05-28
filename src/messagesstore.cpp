#include <QJSValue>
#include <QQmlContext>
#include <QQmlProperty>
#include <QQuickTextDocument>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QTextBoundaryFinder>
#include <QFont>
#include <QGuiApplication>
#include <QDesktopServices>

#include <unicode/urename.h>
#include <unicode/uchar.h>

#include "messagesmodel_p.h"

enum Roles {
    AuthorID = Qt::UserRole,
    AuthorKind,
    Kind,
    Timestamp,
    InReplyTo,

    // Text messages
    Content,

    // Photo messages
    ImageURL,
    ImageCaption,

    // FileMessages
    FileName,
    FileCaption,

    // AddMembers messages
    AddedMembers,

    // Sticker messages
    StickerURL,

    // Video messages
    VideoSize,
    VideoThumbnail,
};

MessagesStore::MessagesStore(Client* parent) : c(parent), d(new Private)
{

}

MessagesStore::~MessagesStore()
{

}

std::pair<TDApi::int53, TDApi::int53> fromVariant(const QVariant& hi)
{
    const auto js = hi.value<QVariantList>();
    return std::make_pair(js[0].toString().toLongLong(), js[1].toString().toLongLong());
}

void MessagesStore::newMessage(TDApi::object_ptr<TDApi::message> msg)
{
    QJsonArray mu;
    mu << QString::number(msg->chat_id_) << QString::number(msg->id_);
    d->messageData[std::make_pair(msg->chat_id_, msg->id_)] = std::move(msg);
    Q_EMIT keyAdded(mu);
}

void MessagesStore::messageIDChange(TDApi::int53 oldID, TDApi::object_ptr<TDApi::message> msg)
{
    deletedMessages(msg->chat_id_, {oldID});
    newMessage(std::move(msg));
}

void MessagesStore::format(const QVariant &key, QQuickTextDocument* doc, QQuickItem *it, bool emojiOnly)
{
    if (!checkKey(key)) {
        return;
    }

    auto mID = fromVariant(key);

    auto content = d->messageData[mID]->content_.get();
    if (content->get_id() != TDApi::messageText::ID) {
        return;
    }

    auto format = static_cast<TDApi::messageText*>(content)->text_.get();

    auto doku = doc->textDocument();
    QTextCursor curs(doku);

    QTextCharFormat cfmt;
    QColor linkColor = QQmlProperty(it, "Kirigami.Theme.linkColor", qmlContext(it)).read().value<QColor>();

    for (const auto& ent : format->entities_) {
        curs.setPosition(ent->offset_, QTextCursor::MoveAnchor);
        curs.setPosition(ent->offset_ + ent->length_, QTextCursor::KeepAnchor);

        using namespace TDApi;

        switch (ent->type_->get_id()) {
        case textEntityTypeBold::ID: {
            cfmt.setFontWeight(QFont::Bold);
            break;
        }
        case textEntityTypeUnderline::ID: {
            cfmt.setFontUnderline(true);
            break;
        }
        case textEntityTypeItalic::ID: {
            cfmt.setFontItalic(true);
            break;
        }
        case textEntityTypeTextUrl::ID: {
            auto it = static_cast<const textEntityTypeTextUrl*>(ent->type_.get());
            cfmt.setForeground(linkColor);
            cfmt.setAnchor(true);
            cfmt.setAnchorHref(QString::fromStdString(it->url_));
            cfmt.setFontUnderline(true);
            break;
        }
        case textEntityTypeUrl::ID: {
            cfmt.setForeground(linkColor);
            cfmt.setAnchor(true);
            cfmt.setAnchorHref(curs.selectedText());
            cfmt.setFontUnderline(true);
            break;
        }
        }
        curs.setCharFormat(cfmt);
    }

    QTextBoundaryFinder finder(QTextBoundaryFinder::Grapheme, doku->toRawText());
    int pos = 0;
    while (finder.toNextBoundary() != -1) {
        auto range = finder.position();

        auto first = doku->toRawText().mid(pos, range-pos).toUcs4()[0];

        if (u_hasBinaryProperty(first, UCHAR_EMOJI_PRESENTATION)) {
            curs.setPosition(pos, QTextCursor::MoveAnchor);
            curs.setPosition(range, QTextCursor::KeepAnchor);

            QTextCharFormat cfmt;
            auto font = QGuiApplication::font();
            font.setFamily("emoji");
            if (emojiOnly) {
                font.setPointSize(font.pointSize()*8);
            } else {
                font.setPointSizeF(font.pointSizeF()*1.2);
            }
            cfmt.setFont(font);

            curs.setCharFormat(cfmt);
        }

        pos = range;
    }

    return;
}

void MessagesStore::deleteMessages(const QString &chatID, const QStringList &messageID)
{
    auto id = chatID.toLongLong();
    TDApi::array<TDApi::int53> messageIDs;
    messageIDs.reserve(messageID.length());
    for (const auto& mID : messageID) {
        messageIDs.push_back(mID.toLongLong());
    }
    c->call<TDApi::deleteMessages>(nullptr, id, messageIDs, true);
}

void MessagesStore::deletedMessages(TDApi::int53 chatID, const TDApi::array<TDApi::int53>& msgIDs)
{
    for (auto mID : msgIDs) {
        d->messageData.erase(std::pair<TDApi::int53,TDApi::int53>(chatID, mID));

        QJsonArray mu;
        mu << QString::number(chatID) << QString::number(mID);
        Q_EMIT keyRemoved(mu);
    }
}

QVariant MessagesStore::data(const QVariant& key, int role)
{
    if (!checkKey(key)) {
        return QVariant();
    }

    auto mID = fromVariant(key);

    switch (Roles(role)) {
    case Roles::InReplyTo:
        return QString::number(d->messageData[mID]->reply_to_message_id_);
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
        if (trueI == -1) {
            return QString();
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
    case Roles::FileCaption: {
        auto content = d->messageData[mID]->content_.get();
        if (content->get_id() != TDApi::messageDocument::ID) {
            return QString();
        }

        auto file = static_cast<TDApi::messageDocument*>(content);
        if (file->caption_ == nullptr) {
            return QString();
        }
        return QString::fromStdString(file->caption_->text_);
    }
    case Roles::FileName: {
        auto content = d->messageData[mID]->content_.get();
        if (content->get_id() != TDApi::messageDocument::ID) {
            return QString();
        }

        auto file = static_cast<TDApi::messageDocument*>(content);
        if (file->document_ == nullptr) {
            return QString();
        }
        return QString::fromStdString(file->document_->file_name_);
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

    case Roles::AuthorKind: {
        using namespace TDApi;

        switch (d->messageData[mID]->sender_->get_id()) {
        case messageSenderChat::ID: return QString("chat");
        case messageSenderUser::ID: return QString("user");
        }
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

    case Roles::AddedMembers: {
        auto it = static_cast<TDApi::messageChatAddMembers*>(d->messageData[mID]->content_.get());
        QVariantList l;
        for (const auto& item : it->member_user_ids_) {
            l << QString::number(item);
        }
        return l;
    }

    case Roles::StickerURL: {
        auto it = static_cast<TDApi::messageSticker*>(d->messageData[mID]->content_.get());
        return QString("image://telegram/%1").arg(it->sticker_->sticker_->id_);
    }

    case Roles::VideoSize: {
        auto it = static_cast<TDApi::messageVideo*>(d->messageData[mID]->content_.get());

        return QSize(it->video_->width_, it->video_->height_);
    }

    case Roles::VideoThumbnail: {
        auto it = static_cast<TDApi::messageVideo*>(d->messageData[mID]->content_.get());

        return QString("image://telegram/%1").arg(it->video_->thumbnail_->file_->id_);
    }

    }

    Q_UNREACHABLE();
}

void MessagesStore::openVideo(const QString& chat, const QString& msg)
{
    auto cID = chat.toLongLong();
    auto mID = msg.toLongLong();
    auto id = std::make_pair(cID, mID);
    if (!d->messageData.contains(id)) {
        return;
    }

    auto it = static_cast<TDApi::messageVideo*>(d->messageData[id]->content_.get());
    auto video = it->video_->video_->id_;

    auto onFinished = [video](qint32 id, QSharedPointer<TDApi::file> file) {
        if (id != video) {
            return;
        }
        if (file->local_ == nullptr) {
            return;
        }
        if (!file->local_->is_downloading_completed_) {
            return;
        }

        auto it = QString::fromStdString(file->local_->path_);
#ifdef Q_OS_LINUX
        QProcess::startDetached("xdg-open", {it});
#else
        QDesktopServices::openUrl(QUrl(it, QUrl::TolerantMode));
#endif
    };
    c->call<TDApi::downloadFile>(
        [onFinished](TDApi::downloadFile::ReturnType t) {
            auto id = t->id_;
            onFinished(id, QSharedPointer<TDApi::file>(t.release()));
        },
        video, 10, 0, 0, true
    );

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
    if (chat == 0 || msg == 0) {
        return;
    }
    c->call<TDApi::getMessage>(
        [](TDApi::getMessage::ReturnType) {},
        chat, msg
    );
}


QHash<int, QByteArray> MessagesStore::roleNames()
{
    QHash<int,QByteArray> roles;

    roles[Roles::AuthorID] = "authorID";
    roles[Roles::AuthorKind] = "authorKind";
    roles[Roles::Kind] = "kind";
    roles[Roles::Timestamp] = "timestamp";
    roles[Roles::InReplyTo] = "inReplyTo";

    roles[Roles::Content] = "content";

    roles[Roles::ImageURL] = "imageURL";
    roles[Roles::ImageCaption] = "imageCaption";

    roles[Roles::FileCaption] = "fileCaption";
    roles[Roles::FileName] = "fileName";

    roles[Roles::AddedMembers] = "addedMembers";

    roles[Roles::StickerURL] = "stickerURL";

    roles[Roles::VideoSize] = "videoSize";
    roles[Roles::VideoThumbnail] = "videoThumbnail";

    return roles;
}
