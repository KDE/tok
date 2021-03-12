#pragma once

#include "chatsmodel.h"

struct ChatsModel::Private
{
    std::vector<std::int32_t> chats;
    std::map<std::int32_t, TDApi::object_ptr<TDApi::chat>> chatData;

    std::optional<int> locateChatIndex(std::int32_t id) const {
        auto v = std::find(chats.cbegin(), chats.cend(), id);

        return v == chats.cend() ? std::optional<int>{} : *v;
    }
};
