// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>

#include <functional>
#include <map>
#include <memory>

#include <QDebug>
#include <QtCore>

namespace TD = td;
namespace TDApi = TD::td_api;

using TObject = TDApi::object_ptr<TDApi::Object>;

#define match(n) { auto& _matchIt = n; switch(n->get_id()) {
#define endmatch } }

#define handleCase(c, v) case c::ID: { auto v = static_cast<c*>(_matchIt.get());
#define endhandle break; }

#define getOrRet(var, thing, retValue) auto var = thing.get(); if (!var) { return retValue; };
