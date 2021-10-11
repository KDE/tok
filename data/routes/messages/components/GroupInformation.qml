// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

import QtQuick.Dialogs 1.0 as Dialogues

import "qrc:/components" as GlobalComponents

QQC2.StackView {

id: theView

clip: true

property Item avatar: theListView.headerItem.avatar

signal readyToDestroy()

initialItem: QQC2.Control {

id: root

padding: 0
topPadding: 0
leftPadding: 0
rightPadding: 0
bottomPadding: 0

Kirigami.Theme.inherit: false
Kirigami.Theme.colorSet: Kirigami.Theme.View

background: Rectangle {
    color: Kirigami.Theme.backgroundColor
}

contentItem: ColumnLayout {
    GlobalComponents.Header {
        RowLayout {
            Layout.leftMargin: !rootRow.shouldUseSidebars ? rootRow.leftOffset+Kirigami.Units.largeSpacing : Kirigami.Units.largeSpacing
            Layout.rightMargin: !rootRow.shouldUseSidebars ? rootRow.rightOffset+Kirigami.Units.largeSpacing : Kirigami.Units.largeSpacing

            QQC2.ToolButton {
                icon.name: "arrow-left"
                onClicked: {
                    theView.visible = false
                    hero.close()
                    destroyTimer.start()
                }
                Timer {
                    id: destroyTimer
                    interval: hero.duration
                    onTriggered: theView.readyToDestroy()
                }
                visible: theView.parent instanceof QQC2.StackView
            }
            Kirigami.Heading {
                level: 4
                text: chatData.key[0] == "-" ? i18n("Group Info") : i18n("Info")

                verticalAlignment: Text.AlignVCenter
                Layout.fillHeight: true
                Layout.margins: Kirigami.Units.largeSpacing
            }
        }
        Layout.fillWidth: true
    }
    QQC2.ScrollView {
        QQC2.ScrollBar.horizontal.policy: QQC2.ScrollBar.AlwaysOff

        ListView {
            id: theListView
            model: tClient.membersModel(chatData.data.mKindID, chatData.data.mKind)
            header: ColumnLayout {
                property alias avatar: theOtherAvatar

                Kirigami.Avatar {
                    id: theOtherAvatar

                    source: chatData.data.mPhoto
                    name: chatData.data.mTitle

                    Layout.preferredWidth: Kirigami.Units.gridUnit * 5
                    Layout.preferredHeight: Kirigami.Units.gridUnit * 5
                    Layout.topMargin: Kirigami.Units.smallSpacing
                    Layout.alignment: Qt.AlignHCenter
                }
                Kirigami.Heading {
                    text: chatData.data.mTitle
                    level: 2

                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.Wrap

                    Layout.leftMargin: Kirigami.Units.largeSpacing
                    Layout.rightMargin: Kirigami.Units.largeSpacing
                    Layout.bottomMargin: Kirigami.Units.largeSpacing
                    Layout.fillWidth: true
                }

                Column {
                    Kirigami.BasicListItem {
                        text: i18n("Photos")
                        reserveSpaceForSubtitle: true

                        onClicked: {
                            theView.push(Qt.resolvedUrl("PhotosView.qml"))
                        }
                        anchors.left: parent.left
                        anchors.right: parent.right
                    }

                    Kirigami.BasicListItem {
                        text: i18n("Videos")
                        reserveSpaceForSubtitle: true

                        onClicked: {
                            theView.push(Qt.resolvedUrl("VideosView.qml"))
                        }
                        anchors.left: parent.left
                        anchors.right: parent.right
                    }

                    Kirigami.BasicListItem {
                        text: i18n("Music")
                        reserveSpaceForSubtitle: true

                        onClicked: {
                            theView.push(Qt.resolvedUrl("MusicView.qml"))
                        }
                        anchors.left: parent.left
                        anchors.right: parent.right
                    }
                    Layout.fillWidth: true
                }

                Item {
                    Layout.bottomMargin: Kirigami.Units.gridUnit * 2
                }

                width: root.width
            }
            delegate: Kirigami.BasicListItem {
                id: del

                required property string userID

                leading: Kirigami.Avatar {
                    name: userData.data.name
                    source: userData.data.smallAvatar

                    width: height
                }

                topPadding: Kirigami.Units.largeSpacing
                bottomPadding: Kirigami.Units.largeSpacing

                text: userData.data.name
                subtitle: userData.data.status
                reserveSpaceForSubtitle: true

                onClicked: {
                    globalUserDataSheet.userID = del.userID
                    globalUserDataSheet.chatID = chatData.key
                    globalUserDataSheet.open()
                }

                Tok.RelationalListener {
                    id: userData

                    model: tClient.userDataModel
                    key: del.userID
                    shape: QtObject {
                        required property string name
                        required property string smallAvatar
                        required property string status
                    }
                }
            }
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        Layout.fillHeight: true
        Layout.fillWidth: true
    }
}

}

}