// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

import "components" as Components
import "qrc:/components" as GlobalComponents

Kirigami.PageRoute {

name: "Messages/NoView"

Kirigami.Page {
    id: pageRoot

    background: Rectangle {
        color: settings.transparent ? Kirigami.ColorUtils.scaleColor("transparent", {"alpha": -80}) : Kirigami.Theme.backgroundColor
    }

    QQC2.Label {
        text: i18n("Select a chat to start messaging")
        anchors.centerIn: parent
        padding: Kirigami.Units.smallSpacing
        leftPadding: Kirigami.Units.smallSpacing*2
        rightPadding: Kirigami.Units.smallSpacing*2

        background: Rectangle {
            radius: height
            Kirigami.Theme.inherit: false
            Kirigami.Theme.colorSet: Kirigami.Theme.Button
            color: Kirigami.Theme.backgroundColor
        }
    }
}

}
