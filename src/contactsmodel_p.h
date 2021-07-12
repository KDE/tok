// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "contactsmodel.h"

struct ContactsModel::Private
{
    TDApi::object_ptr<TDApi::users> data;
    QSet<TDApi::int53> selectedIDs;
};
