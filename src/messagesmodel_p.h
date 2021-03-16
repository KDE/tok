#pragma once

#include <deque>

#include "messagesmodel.h"

struct MessagesModel::Private
{
    std::deque<TDApi::int53> messages;

    TDApi::int53 id;
};

struct MessagesStore::Private
{
    std::map<std::pair<TDApi::int53, TDApi::int53>, TDApi::object_ptr<TDApi::message>> messageData;
};
