#pragma once

#include <deque>

#include "messagesmodel.h"

struct MessagesModel::Private
{
    std::deque<TDApi::int53> messages;
    std::map<TDApi::int53, TDApi::object_ptr<TDApi::message>> messageData;

    TDApi::int53 id;
};
