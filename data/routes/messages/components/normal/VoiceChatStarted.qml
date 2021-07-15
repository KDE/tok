// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

Item {
    id: msg

    implicitWidth: del.recommendedSize
    implicitHeight: header.implicitHeight
    Layout.leftMargin: Kirigami.Units.largeSpacing

    readonly property string text: i18n("%1 started a voice chat", userData.data.name)

    Kirigami.Heading {
        id: header

        level: 4

        text: msg.text

        padding: Kirigami.Units.smallSpacing
        leftPadding: Kirigami.Units.largeSpacing
        rightPadding: Kirigami.Units.largeSpacing

        anchors.centerIn: parent

        background: Rectangle {
            radius: height

            Kirigami.Theme.colorSet: Kirigami.Theme.Window
            color: Kirigami.Theme.backgroundColor
        }
    }
}