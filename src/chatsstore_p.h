#pragma once

#include "chatsstore.h"

struct ChatsStore::Private {
    std::map<std::int32_t, TDApi::object_ptr<TDApi::chat>> chatData;
};
