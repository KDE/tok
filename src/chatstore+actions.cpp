// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <KLocalizedString>

#include "chatsstore_p.h"
#include "userdata.h"
#include "internationalisedlist.h"

template<typename T, typename O>
static QList<O> map(const QList<T>& input, std::function<O(T)> mapper) {
    QList<O> o;
    o.reserve(input.length());

    for (const auto& item : input) {
        o << mapper(item);
    }

    return o;
}

template<typename T, typename O>
static QList<T> takeFirst(const QList<QPair<T, O>>& in) {
    QList<T> o;
    o.reserve(in.length());

    for (const auto& item : in) {
        o << item.first;
    }

    return o;
}

QJsonValue ChatsStore::prepare(TDApi::int53 chatID)
{
    using namespace TDApi;

    QList<QPair<TDApi::int53, const chatActionTyping*>> listOfChatActionTyping;
    QList<QPair<TDApi::int53, const chatActionUploadingDocument*>> listOfChatActionUploadingDocument;
    QList<QPair<TDApi::int53, const chatActionUploadingPhoto*>> listOfChatActionUploadingPhoto;
    QList<QPair<TDApi::int53, const chatActionUploadingVideo*>> listOfChatActionUploadingVideo;
    QList<QPair<TDApi::int53, const chatActionUploadingVideoNote*>> listOfChatActionUploadingVideoNote;
    QList<QPair<TDApi::int53, const chatActionUploadingVoiceNote*>> listOfChatActionUploadingVoiceNote;
    QList<QPair<TDApi::int53, const chatActionRecordingVideo*>> listOfChatActionRecordingVideo;
    QList<QPair<TDApi::int53, const chatActionRecordingVideoNote*>> listOfChatActionRecordingVideoNote;
    QList<QPair<TDApi::int53, const chatActionRecordingVoiceNote*>> listOfChatActionRecordingVoiceNote;

    QJsonObject it;
    QStringList messages;

    auto username = [this](int53 userID) {
        if (!c->userDataModel()->userData.contains(userID)) {
            return QString();
        }
        return QString::fromStdString(c->userDataModel()->userData[userID]->first_name_);
    };

    const auto& r = d->ensure(chatID);

    for (const auto& item : r) {
        switch (item.second->get_id()) {
        case chatActionTyping::ID: { listOfChatActionTyping << qMakePair(item.first, static_cast<const chatActionTyping*>(item.second.get())); break; }
        case chatActionUploadingDocument::ID: { listOfChatActionUploadingDocument << qMakePair(item.first, static_cast<const chatActionUploadingDocument*>(item.second.get())); break; }
        case chatActionUploadingPhoto::ID: { listOfChatActionUploadingPhoto << qMakePair(item.first, static_cast<const chatActionUploadingPhoto*>(item.second.get())); break; }
        case chatActionUploadingVideo::ID: { listOfChatActionUploadingVideo << qMakePair(item.first, static_cast<const chatActionUploadingVideo*>(item.second.get())); break; }
        case chatActionUploadingVideoNote::ID: { listOfChatActionUploadingVideoNote << qMakePair(item.first, static_cast<const chatActionUploadingVideoNote*>(item.second.get())); break; }
        case chatActionUploadingVoiceNote::ID: { listOfChatActionUploadingVoiceNote << qMakePair(item.first, static_cast<const chatActionUploadingVoiceNote*>(item.second.get())); break; }
        case chatActionRecordingVideo::ID: { listOfChatActionRecordingVideo << qMakePair(item.first, static_cast<const chatActionRecordingVideo*>(item.second.get())); break; }
        case chatActionRecordingVideoNote::ID: { listOfChatActionRecordingVideoNote << qMakePair(item.first, static_cast<const chatActionRecordingVideoNote*>(item.second.get())); break; }
        case chatActionRecordingVoiceNote::ID: { listOfChatActionRecordingVoiceNote << qMakePair(item.first, static_cast<const chatActionRecordingVoiceNote*>(item.second.get())); break; }
        }
    }

    if (listOfChatActionTyping.length() > 0) {
        auto userIDs = takeFirst(listOfChatActionTyping);
        auto userNames = map<int53, QString>(userIDs, username);
        auto internationalised = internationalisedList(userNames);

        messages << i18np("%1 is typing", "%1 are typing", internationalised, userNames.length());
    }
    if (listOfChatActionUploadingDocument.length() > 0) {
        auto userIDs = takeFirst(listOfChatActionUploadingDocument);
        auto userNames = map<int53, QString>(userIDs, username);
        auto internationalised = internationalisedList(userNames);

        messages << i18np("%1 is uploading a file", "%1 are uploading files", internationalised, userNames.length());
    }
    if (listOfChatActionUploadingPhoto.length() > 0) {
        auto userIDs = takeFirst(listOfChatActionUploadingPhoto);
        auto userNames = map<int53, QString>(userIDs, username);
        auto internationalised = internationalisedList(userNames);

        messages << i18np("%1 is uploading a photo", "%1 are uploading photos", internationalised, userNames.length());
    }
    if (listOfChatActionUploadingVideo.length() > 0) {
        auto userIDs = takeFirst(listOfChatActionUploadingVideo);
        auto userNames = map<int53, QString>(userIDs, username);
        auto internationalised = internationalisedList(userNames);

        messages << i18np("%1 is uploading a video", "%1 are uploading videos", internationalised, userNames.length());
    }
    if (listOfChatActionUploadingVideoNote.length() > 0) {
        auto userIDs = takeFirst(listOfChatActionUploadingVideoNote);
        auto userNames = map<int53, QString>(userIDs, username);
        auto internationalised = internationalisedList(userNames);

        messages << i18np("%1 is uploading a video note", "%1 are uploading video notes", internationalised, userNames.length());
    }
    if (listOfChatActionUploadingVoiceNote.length() > 0) {
        auto userIDs = takeFirst(listOfChatActionUploadingVoiceNote);
        auto userNames = map<int53, QString>(userIDs, username);
        auto internationalised = internationalisedList(userNames);

        messages << i18np("%1 is uploading a voice note", "%1 are uploading voice notes", internationalised, userNames.length());
    }
    if (listOfChatActionRecordingVideo.length() > 0) {
        auto userIDs = takeFirst(listOfChatActionRecordingVideo);
        auto userNames = map<int53, QString>(userIDs, username);
        auto internationalised = internationalisedList(userNames);

        messages << i18np("%1 is recording a video", "%1 are recording videos", internationalised, userNames.length());
    }
    if (listOfChatActionRecordingVideoNote.length() > 0) {
        auto userIDs = takeFirst(listOfChatActionRecordingVideoNote);
        auto userNames = map<int53, QString>(userIDs, username);
        auto internationalised = internationalisedList(userNames);

        messages << i18np("%1 is recording a video note", "%1 are recording video notes", internationalised, userNames.length());
    }
    if (listOfChatActionRecordingVoiceNote.length() > 0) {
        auto userIDs = takeFirst(listOfChatActionRecordingVoiceNote);
        auto userNames = map<int53, QString>(userIDs, username);
        auto internationalised = internationalisedList(userNames);

        messages << i18np("%1 is recording a voice note", "%1 are recording voice notes", internationalised, userNames.length());
    }

    it["any"] = (messages.length() > 0);
    if (messages.length() > 0) {
        it["message"] = internationalisedList(messages) + "...";
    }
    return it;
}
