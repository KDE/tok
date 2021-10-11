// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: AGPL-3.0-or-later

import QtQuick 2.10
import QtQuick.Layouts 1.10
import org.kde.kirigami 2.14 as Kirigami
import QtQuick.Controls 2.10 as QQC2
import org.kde.Tok 1.0 as Tok

Loader {
    id: __loader

    active: false
    required property string userID
    required property string chatID

    function open() {
        if (!active) {
            active = true
        } else {
            this.item.open()
        }
    }

    onLoaded: this.item.open()
    sourceComponent: Kirigami.OverlaySheet {
        id: __overlaySheet

        parent: rootWindow

        property Tok.RelationalListener mu: Tok.RelationalListener {
            id: userData

            model: tClient.userDataModel
            key: __loader.userID
            shape: QtObject {
                required property string name
                required property string smallAvatar
                required property string bio
            }
        }

        property Tok.RelationalListener mu2: Tok.RelationalListener {
            id: chatData

            model: tClient.chatsStore
            key: __loader.chatID
            shape: QtObject {
                required property string mTitle
                required property string mKind
                required property variant mOwnStatus
            }

            readonly property bool isGroup: chatData.data.mKind === "basicGroup" || chatData.data.mKind === "superGroup"
        }

        ColumnLayout {
            Kirigami.Avatar {
                name: userData.data.name
                source: userData.data.smallAvatar
                implicitHeight: 64
                implicitWidth: 64

                Layout.alignment: Qt.AlignHCenter
            }
            Kirigami.Heading {
                level: 2

                text: userData.data.name
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignHCenter

                Layout.fillWidth: true
                Layout.maximumWidth: Kirigami.Units.gridUnit * 20
            }
            QQC2.Label {
                text: userData.data.bio
                visible: userData.data.bio !== ""
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignHCenter

                Layout.fillWidth: true
                Layout.maximumWidth: Kirigami.Units.gridUnit * 20
            }

            QQC2.Button {
                text: i18n("Ban from \"%1\"", chatData.data.mTitle)
                enabled: chatData.data.mOwnStatus.canRemove
                onClicked: {
                    tClient.chatsStore.setStatus(__loader.chatID, __loader.userID, {"status": "banned"})
                    __overlaySheet.close()
                }
                visible: chatData.isGroup

                Layout.fillWidth: true
                Layout.maximumWidth: Kirigami.Units.gridUnit * 20
            }

            QQC2.Button {
                text: i18n("Kick from \"%1\"", chatData.data.mTitle)
                enabled: chatData.data.mOwnStatus.canRemove
                onClicked: {
                    tClient.chatsStore.setStatus(__loader.chatID, __loader.userID, {"status": "kicked"})
                    __overlaySheet.close()
                }
                visible: chatData.isGroup

                Layout.fillWidth: true
                Layout.maximumWidth: Kirigami.Units.gridUnit * 20
            }
        }
    }
}
