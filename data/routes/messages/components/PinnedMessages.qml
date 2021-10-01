// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.10
import org.kde.Tok 1.0 as Tok
import org.kde.kirigami 2.15 as Kirigami
import QtQuick.Dialogs 1.3
import "qrc:/components" as GlobalComponents
import QtMultimedia 5.15
import "qrc:/components/upload" as UploadComponents
import QtQuick.Window 2.15

Window {
    id: _pinnedMessagesDialogDesktop

    title: i18nc("dialog title", "Pinned Messages in %1", chatData.data.mTitle)
    width: Math.max(contItem.implicitWidth, Kirigami.Units.gridUnit * 25)
    height: Math.max(contItem.implicitHeight, Kirigami.Units.gridUnit * 40)

    minimumWidth: Kirigami.Units.gridUnit * 20
    minimumHeight: Kirigami.Units.gridUnit * 20

    Tok.RelationalListener {
        id: chatData

        model: tClient.chatsStore
        key: _pinnedMessagesDialogDesktop.chatID
        shape: QtObject {
            required property string mTitle
        }
    }

    required property string chatID

    QQC2.Control {
        id: contItem
        anchors.fill: parent
        topPadding: 0
        leftPadding: 0
        rightPadding: 0
        bottomPadding: 0
        background: Rectangle {
            Kirigami.Separator {
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                }
            }
            color: Kirigami.Theme.backgroundColor
        }
        contentItem: GridLayout {
                columns: 1
                rows: 3

            GlobalComponents.Header {
                visible: Kirigami.Settings.isMobile

                RowLayout {
                    Layout.row: 0

                    QQC2.ToolButton {
                        icon.name: "arrow-left"
                        onClicked: _pinnedMessagesDialogDesktop.close()
                    }
                    Kirigami.Heading {
                        level: 4
                        text: i18nc("dialog title", "Pinned Messages in %1", _pinnedMessagesDialogDesktop.title)

                        verticalAlignment: Text.AlignVCenter
                        Layout.fillHeight: true
                        Layout.margins: Kirigami.Units.largeSpacing
                    }
                }
                Layout.fillWidth: true
            }

            QQC2.ScrollView {
            ListView {
                clip: true
                model: tClient.searchMessagesModel({
                    "chatID": _pinnedMessagesDialogDesktop.chatID,
                    "kind": "pinned",
                })
                delegate: MessageDelegate {
                    id: del

                    required property string messageID

                    menuEnabled: false
                    mID: messageID
                    mChatID: _pinnedMessagesDialogDesktop.chatID
                    mNextID: ""
                    mPreviousID: ""

                    afterMessage: [
                        QQC2.Button {
                            icon.name: "quickview"

                            QQC2.ToolTip.text: i18n("View In Chat")
                            QQC2.ToolTip.visible: hovered

                            enabled: _pinnedMessagesDialogDesktop.chatID === messagesViewRoot.chatID

                            onClicked: lView.hopToID(del.messageID)
                        }
                    ]
                }
                Layout.preferredWidth: Kirigami.Units.gridUnit * 25
                Layout.preferredHeight: Kirigami.Units.gridUnit * 40
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
                Layout.row: Kirigami.Settings.isMobile ? 1 : 2

                Layout.preferredWidth: Kirigami.Units.gridUnit * 25
                Layout.preferredHeight: Kirigami.Units.gridUnit * 40
                Layout.fillWidth: true
                Layout.fillHeight: true

                QQC2.ScrollBar.horizontal.policy: QQC2.ScrollBar.AlwaysOff
            }
        }
    }
}

