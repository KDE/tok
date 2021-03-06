// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtGraphicalEffects 1.15
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

Item {
    id: msg

    implicitWidth: del.recommendedSize
    implicitHeight: header.implicitHeight
    Layout.leftMargin: Kirigami.Units.largeSpacing

    readonly property bool isOwn: addData.data.addedMembers.length == 1 && addData.data.addedMembers[0] == messageData.data.authorID
    readonly property string text: {
        if (isOwn) {
            return i18n("%1 joined the group", userData.data.name)
        }

        return "Unsupported"
    }

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

            layer.enabled: true
            layer.effect: DropShadow {
                cached: true
                horizontalOffset: 0
                verticalOffset: 1
                radius: 2.0
                samples: 17
                color: "#30000000"
            }
        }
    }

    Tok.RelationalListener {
        id: addData

        model: tClient.messagesStore
        key: [del.mChatID, del.mID]
        shape: QtObject {
            required property var addedMembers
        }
    }
}