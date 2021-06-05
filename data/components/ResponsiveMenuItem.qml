// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.10

QtObject {
    property string text: ""
    property bool enabled: true
    property bool visible: true
    signal triggered()
}
