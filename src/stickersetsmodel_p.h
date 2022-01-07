#pragma once

#include "stickersetsmodel.h"

struct StickerSetsModel::Private
{
    TDApi::array<TDApi::int64> stickerIDs;
};