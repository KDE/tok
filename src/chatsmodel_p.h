// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "chatsmodel.h"

struct ChatsModel::Private {
    std::vector<TDApi::int53> chats;

    // void filterChats()
    // {
    //     (void) std::remove_if(chats.begin(), chats.end(), [this](std::int32_t it) -> bool {
    //         return chatData[it]->positions_[0] == nullptr;
    //     });
    // }
    // void sortChats()
    // {
    //     std::sort(chats.begin(), chats.end(), [this](std::int32_t lhs, std::int32_t rhs) -> bool {
    //         return std::make_pair(chatData[lhs]->positions_[0]->order_, chatData[lhs]->id_)
    //             <= std::make_pair(chatData[rhs]->positions_[0]->order_, chatData[rhs]->id_);
    //     });
    // }
    // auto locateChatIndex(std::int32_t id) const
    // {
    //     auto v = std::find(chats.cbegin(), chats.cend(), id);

    //     return v - chats.cbegin();
    // }

    bool atEnd = true;
    int numLoadedChats = 0;
};
