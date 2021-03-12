#pragma once

#include "chatsmodel.h"

struct ChatsModel::Private
{
    std::vector<std::int32_t> chats;
    std::map<std::int32_t, TDApi::object_ptr<TDApi::chat>> chatData;
};
