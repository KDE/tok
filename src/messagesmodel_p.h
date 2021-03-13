#pragma once

#include <deque>

#include "messagesmodel.h"

struct MessagesModel::Private
{
    std::deque<std::int32_t> messages;
    std::map<std::int32_t, TDApi::object_ptr<TDApi::message>> messageData;

    TDApi::int53 id;
};
