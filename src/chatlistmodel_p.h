#pragma once

#include "chatlistmodel.h"

struct ChatListModel::Private
{
    TDApi::array<TDApi::object_ptr<TDApi::chatFilterInfo>> chats;
};