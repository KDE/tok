// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtGraphicalEffects 1.15
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.12 as Kirigami
import org.kde.Tok 1.0 as Tok

import QtPositioning 5.15
import QtLocation 5.15

import "qrc:/components" as Components

QQC2.Control {
    id: venueRoot

    topPadding: 0
    bottomPadding: 0
    leftPadding: tailSize
    rightPadding: 0

    readonly property int tailSize: Kirigami.Units.largeSpacing

    Layout.maximumWidth: del.recommendedSize
    Layout.preferredWidth: del.recommendedSize

    background: MessageBackground {
        id: _background
        tailSize: venueRoot.tailSize

        anchors.fill: parent
    }

    contentItem: ColumnLayout {
        ReplyBlock {
            Layout.topMargin: Kirigami.Units.smallSpacing
            Layout.bottomMargin: Kirigami.Units.smallSpacing
            Layout.leftMargin: Kirigami.Units.largeSpacing
            Layout.rightMargin: Kirigami.Units.largeSpacing
        }
        Map {
            id: map

            Layout.preferredHeight: 300
            Layout.fillWidth: true

            center: QtPositioning.coordinate(mapData.data.placeLocation.x, mapData.data.placeLocation.y)
            zoomLevel: 15
            plugin: Plugin {
                name: "osm"
            }

            QQC2.Label {
                text: messageData.data.timestamp

                Kirigami.Theme.colorSet: Kirigami.Theme.Complementary
                Kirigami.Theme.inherit: false

                font.pointSize: -1
                font.pixelSize: Kirigami.Units.gridUnit * (2/3)

                padding: Kirigami.Units.smallSpacing
                leftPadding: Math.floor(Kirigami.Units.smallSpacing*(3/2))
                rightPadding: Math.floor(Kirigami.Units.smallSpacing*(3/2))

                anchors {
                    bottom: parent.bottom
                    right: parent.right
                    margins: Kirigami.Units.largeSpacing
                }
                background: Rectangle {
                    color: Kirigami.Theme.backgroundColor
                    opacity: 0.7
                    radius: 3
                }
            }
        }
        Tok.RelationalListener {
            id: mapData

            model: tClient.messagesStore
            key: [del.mChatID, del.mID]
            shape: QtObject {
                required property point venueLocation
            }
        }

        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                color: "red"
                radius: 4
                width: map.width
                height: map.height
            }
        }
    }
}
