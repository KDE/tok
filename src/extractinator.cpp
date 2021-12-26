// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <KLocalizedString>

#include "userdata.h"

#include "extractinator.h"

QString Extractinator::extractAuthor(Client* c, TDApi::message* msg)
{
    using namespace TDApi;

    match (msg->sender_id_)
        handleCase(messageSenderUser, user)
            getOrRet(data, c->userDataModel()->userData[user->user_id_], i18nc("we know that this person is a human, but we don't know their name", "Unknown Sender"));

            return QStringList{QString::fromStdString(data->first_name_),QString::fromStdString(data->last_name_)}.join(" ").trimmed();
        endhandle
    endmatch

    return i18nc("we don't know what kind of user sent this message", "Unsupported");
}

inline QString operator*(const TDApi::string& op)
{
    return QString::fromStdString(op);
}

QString Extractinator::extractBody(Client* c, TDApi::message* msg)
{
    using namespace TDApi;

    QString ret;

    match(msg->content_)
        handleCase(messageText, msg)
            ret = QString::fromStdString(msg->text_->text_);
        endhandle
        handleCase(messageAnimation, msg)
            if (msg->caption_->text_.length() > 0) {
                ret = i18nc("notification body for receiving a gif with a caption", "GIF: %1", *msg->caption_->text_);
            } else {
                ret = i18nc("notification body for receiving a gif", "GIF");
            }
        endhandle
        handleCase(messageAudio, msg)
            auto fn = msg->audio_->title_.length() > 0 ? msg->audio_->title_ : msg->audio_->file_name_;
            if (msg->caption_->text_.length() > 0) {
                ret = i18nc("notification body for receiving a music with a caption", "Music (%1): %2", *fn, *msg->caption_->text_);
            } else {
                ret = i18nc("notification body for receiving music", "Music (%1)", *fn);
            }
        endhandle
        handleCase(messageDocument, msg)
            auto fn = msg->document_->file_name_;
            if (msg->caption_->text_.length() > 0) {
                ret = i18nc("notification body for receiving a file with a caption", "File (%1): %2", *fn, *msg->caption_->text_);
            } else {
                ret = i18nc("notification body for receiving file", "File (%1)", *fn);
            }
        endhandle
        handleCase(messagePhoto, msg)
            if (msg->caption_->text_.length() > 0) {
                ret = i18nc("notification body for receiving a photo with a caption", "Photo: %1", *msg->caption_->text_);
            } else {
                ret = i18nc("notification body for receiving a photo", "Photo");
            }
        endhandle
        handleCase(messageExpiredPhoto, msg)
            Q_UNUSED(msg)

            ret = i18nc("notification body for receiving an expired photo (one that self-destructed)", "Expired Photo");
        endhandle
        handleCase(messageSticker, msg)
            ret = i18nc("notification body for receiving a sticker", "Sticker %1", *msg->sticker_->emoji_);
        endhandle
        handleCase(messageVideo, msg)
            if (msg->caption_->text_.length() > 0) {
                ret = i18nc("notification body for receiving a video with a caption", "Video: %1", *msg->caption_->text_);
            } else {
                ret = i18nc("notification body for receiving a video", "Video");
            }
        endhandle
        handleCase(messageExpiredVideo, msg)
            Q_UNUSED(msg)

            ret = i18nc("notification body for receiving an expired video (one that self-destructed)", "Expired Video");
        endhandle
        handleCase(messageVideoNote, msg)
            Q_UNUSED(msg)

            ret = i18nc("notification body for receiving a video note", "Video");
        endhandle
        handleCase(messageVoiceNote, msg)
            Q_UNUSED(msg)

            ret = i18nc("notification body for receiving a voice note", "Video");
        endhandle
        handleCase(messageLocation, msg)
            Q_UNUSED(msg)

            ret = i18nc("notification body for receiving a location", "Location");
        endhandle
        handleCase(messageVenue, msg)
            ret = i18nc("notification body for receiving a venue", "Venue: %1", *msg->venue_->title_);
        endhandle
        handleCase(messageContact, msg)
            ret = i18nc("notification body for receiving a contact, %1 %2 are firstname lastname from telegram's system", "Contact: %1 %2", *msg->contact_->first_name_, *msg->contact_->last_name_);
        endhandle
        handleCase(messageDice, msg)
            ret = i18nc("notification body for receiving a dice", "%1", *msg->emoji_);
        endhandle
        handleCase(messageGame, msg)
            ret = i18nc("notification body for receiving a game", "Game: %1", *msg->game_->title_);
        endhandle
        handleCase(messagePoll, msg)
            ret = i18nc("notification body for receiving a poll", "Poll: %1", *msg->poll_->question_);
        endhandle
        handleCase(messageInvoice, msg)
            ret = i18nc("notification body for receiving an invoice", "Invoice: %1", *msg->title_);
        endhandle
        default: {
            ret = "Unsupported";
            break;
        }
    endmatch

    return ret;
}

QuickGlance Extractinator::extract(Client* c, TDApi::message* msg)
{
    return QuickGlance { extractAuthor(c, msg), extractBody(c, msg) };
}
