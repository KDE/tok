// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <KColorSchemeManager>
#include <QAbstractItemModel>

#include "colorschemer.h"

ColorSchemer::ColorSchemer(QObject* parent) : QObject(parent), c(new KColorSchemeManager(this))
{
}

ColorSchemer::~ColorSchemer()
{

}

QAbstractItemModel* ColorSchemer::model() const
{
    auto it = new RoleNames;
    it->setSourceModel(c->model());

    return it;
}

void ColorSchemer::apply(int idx)
{
    c->activateScheme(c->model()->index(idx, 0));
}

QHash<int,QByteArray> RoleNames::roleNames() const
{
    return {
        { Qt::DisplayRole, "colorSchemeName" }
    };
}
