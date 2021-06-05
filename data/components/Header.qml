// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Layouts 1.10
import org.kde.kirigami 2.10 as Kirigami

Kirigami.AbstractApplicationHeader {
    preferredHeight: Math.max(colLayout.implicitHeight, Math.round(Kirigami.Units.gridUnit * 2.5))

    default property alias it: colLayout.data

    TapHandler {
        onPressedChanged: if (pressed) rootWindow.startSystemMove()
    }

    contentItem: ColumnLayout {
        id: colLayout
        anchors.fill: parent
    }
}