// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QString>

#include "client.h"

struct QuickGlance
{
    QString author;
    QString body;
};

class Extractinator
{

private:
    static QString extractAuthor(Client* c, TDApi::message* msg);
    static QString extractBody(Client* c, TDApi::message* msg);

public:
    static QuickGlance extract(Client* c, TDApi::message* msg);

};