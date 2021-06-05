// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "membersmodel.h"
struct MembersModel::Private
{
    TDApi::int53 chatID = 0;

    TDApi::array<TDApi::object_ptr<TDApi::chatMember>> members;

    bool isSupergroup = false;
    bool atEnd = false;
};
