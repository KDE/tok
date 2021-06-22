// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

pragma Singleton
import QtMultimedia 5.15

Audio {
    objectName: "hallöchen"

    property string thumbnail: ""
    property string audioID: ""

    function clear() {
        thumbnail = ""
        audioID = ""
        source = ""
    }
}
