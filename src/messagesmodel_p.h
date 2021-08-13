// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <deque>

#include "messagesmodel.h"

struct MessagesModel::Private
{
    std::deque<TDApi::int53> messages;

    TDApi::int53 id;

    bool isFetchingBack = false;
    bool canFetchBack = true;
};

struct MessagesStore::Private
{
    std::map<std::pair<TDApi::int53, TDApi::int53>, TDApi::object_ptr<TDApi::message>> messageData;
};

struct MessagesModel::SendData
{
    struct Text { TDApi::object_ptr<TDApi::formattedText> s; };
    struct Photo { TDApi::object_ptr<TDApi::formattedText> s; QUrl p; };
    struct File { TDApi::object_ptr<TDApi::formattedText> s; QUrl p; };
    struct Video { TDApi::object_ptr<TDApi::formattedText> s; QUrl p; };
    struct Audio { TDApi::object_ptr<TDApi::formattedText> s; QUrl p; };

    std::variant<Text,Photo,File,Video,Audio> contents;
    TDApi::int53 replyToID;
};
