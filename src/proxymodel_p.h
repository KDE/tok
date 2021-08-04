// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "proxymodel.h"

struct ProxyModel::Private
{
    TDApi::array<TDApi::object_ptr<TDApi::proxy>> proxies;
};
