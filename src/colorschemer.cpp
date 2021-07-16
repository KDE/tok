// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <KColorSchemeManager>
#include <QAbstractItemModel>
#include <QSettings>

#include "colorschemer.h"

ColorSchemer::ColorSchemer(QObject* parent) : QObject(parent), c(new KColorSchemeManager(this))
{
    QSettings sets;
    auto it = sets.value("color-scheme");
    if (it.isValid()) {
        auto scheme = it.toString();
        c->activateScheme(c->indexForScheme(scheme));
    }
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
    QSettings sets;
    sets.setValue("color-scheme", c->model()->data(c->model()->index(idx, 0), Qt::DisplayRole));

    c->activateScheme(c->model()->index(idx, 0));
}

QHash<int,QByteArray> RoleNames::roleNames() const
{
    return {
        { Qt::DisplayRole, "colorSchemeName" }
    };
}
