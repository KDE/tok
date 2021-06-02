#pragma once

#include "membersmodel.h"
struct MembersModel::Private
{
    TDApi::int53 chatID = 0;

    TDApi::array<TDApi::object_ptr<TDApi::chatMember>> members;

    bool isSupergroup = false;
    bool atEnd = false;
};
