#pragma once

#include "recentstickers.h"

struct RecentStickersModel::Private
{
    TDApi::array<TDApi::object_ptr<TDApi::sticker>> data;
};
