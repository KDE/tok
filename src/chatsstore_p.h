// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "chatsstore.h"

struct ChatsStore::Private {
    std::map<TDApi::int53, TDApi::object_ptr<TDApi::chat>> chatData;
};
