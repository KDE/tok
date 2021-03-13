#pragma once

#include "messagesmodel.h"

struct MessagesModel::Private
{
    std::vector<std::int32_t> messages;
    std::map<std::int32_t, TDApi::object_ptr<TDApi::message>> messageData;

    TDApi::int53 id;
};
