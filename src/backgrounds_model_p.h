#pragma once

#include "backgrounds_model.h"

struct BackgroundsModel::Private
{
    TDApi::object_ptr<TDApi::backgrounds> bgs;
};
