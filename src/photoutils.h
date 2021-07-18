// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QString>
#include <QCoreApplication>
#include <QDebug>

#include "defs.h"

QString imageToURL(const TDApi::object_ptr<TDApi::file>& file);

