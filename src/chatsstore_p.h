#pragma once

#include "chatsstore.h"

struct ChatsStore::Private {
    std::map<TDApi::int53, TDApi::object_ptr<TDApi::chat>> chatData;
};
