#pragma once

#include "stickersetsstore.h"

struct StickerSetsStore::Private
{
    std::map<TDApi::int64, TDApi::object_ptr<TDApi::stickerSet>> stickerSetData;
    std::map<TDApi::int64, QVariantList> stickerData;
};
