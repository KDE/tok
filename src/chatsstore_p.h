// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "chatsstore.h"

struct PermsData
{
    Q_GADGET
    Q_PROPERTY(bool canRemove MEMBER canRemove)

public:
    bool canRemove = false;
};

struct ChatsStore::Private {
    std::map<TDApi::int53, TDApi::object_ptr<TDApi::chat>> chatData;
    std::map<TDApi::int53, std::variant<TDApi::object_ptr<TDApi::basicGroup>, TDApi::object_ptr<TDApi::supergroup>>> extendedData;
    std::map<TDApi::int53, std::map<TDApi::int53,TDApi::object_ptr<TDApi::ChatAction>>> chatActions;
    std::map<TDApi::int53,TDApi::object_ptr<TDApi::ChatAction>>& ensure(TDApi::int53 cid) {
        if (!chatActions.contains(cid)) {
            chatActions[cid] = std::map<TDApi::int53,TDApi::object_ptr<TDApi::ChatAction>>();
        }
        return chatActions[cid];
    }
};
