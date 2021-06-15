// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QDebug>

#include <KLocalizedString>

#include "internationalisedlist.h"

QString internationalisedList(const QStringList& it)
{
    Q_ASSERT(it.length() >= 1);

    if (it.length() == 1) {
        return it[0];
    }

    if (it.length() == 2) {
        return i18nc("A list of two items. The items can be used to format any sort of nouns, e.g. a list of people, places, things.", "%1 and %2", it[0], it[1]);
    }

    auto mut = it;

    auto last = mut.takeLast();
    auto slast = mut.takeLast();

    auto b = i18nc("The end of a list of 3 or more items. In English, this is '{0}, and {1}'.", "%1, and %2", slast, last);

    while (mut.length() > 1) {
        b = i18nc("The middle of a list of 4 or more items. In English, this is '{0}, {1}'", "%1, %2", mut.takeLast());
    }

    return i18nc("The start of a list of 3 or more items. In English, this is '{0}, {1}'", "%1, %2", mut.takeLast(), b);
}
