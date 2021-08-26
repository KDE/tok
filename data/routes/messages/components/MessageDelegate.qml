// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok
import QtGraphicalEffects 1.15

import "qrc:/components" as GlobalComponents

QQC2.Control {
    id: del

    signal edit()

    required property string mID
    required property string mChatID
    required property string mNextID
    required property string mPreviousID

    readonly property int recommendedSize: (rootRow.shouldUseSidebars ? Math.max(del.width / 3, Kirigami.Units.gridUnit * 15) : Math.min(del.width * 0.8, Kirigami.Units.gridUnit * 15))
    readonly property int recommendedSmallSize: (rootRow.shouldUseSidebars ? Math.max(del.width / 5, Kirigami.Units.gridUnit * 10) : Math.min(del.width * 0.6, Kirigami.Units.gridUnit * 10))

    readonly property bool isOwnMessage: messageData.data.authorID === tClient.ownID
    readonly property bool showAvatar: !serviceMessage && (nextData.data.authorID != messageData.data.authorID) && (!(Kirigami.Settings.isMobile && isOwnMessage))
    readonly property bool serviceMessage: {
        return messageData.data.kind == "messageChatAddMembers"
    }
    readonly property bool separateFromPrevious: previousData.data.authorID != messageData.data.authorID
    readonly property bool canDeleteMessage: messageData.data.canDeleteForSelf || messageData.data.canDeleteForOthers

    topPadding: settings.slimMode ? (del.separateFromPrevious ? Kirigami.Units.smallSpacing : 0) : (del.separateFromPrevious ? Kirigami.Units.largeSpacing : Kirigami.Units.smallSpacing)
    bottomPadding: 0
    z: 10

    Accessible.role: Accessible.ListItem
    Accessible.name: tryit(() => __loaderSwitch.item.Accessible.name)
    readonly property bool showFocusRing: true

    Kirigami.Theme.backgroundColor: {
        if (isOwnMessage)
            return Kirigami.ColorUtils.tintWithAlpha(messagesViewRoot.Kirigami.Theme.backgroundColor, messagesViewRoot.Kirigami.Theme.focusColor, 0.2)

        if (Kirigami.ColorUtils.brightnessForColor(messagesViewRoot.Kirigami.Theme.backgroundColor) == Kirigami.ColorUtils.Light)
            return Qt.darker(messagesViewRoot.Kirigami.Theme.backgroundColor, 1.1)
        else
            return Qt.lighter(messagesViewRoot.Kirigami.Theme.backgroundColor, 1.3)
    }

    readonly property color nestedButtonColor: {
        if (Kirigami.ColorUtils.brightnessForColor(messagesViewRoot.Kirigami.Theme.backgroundColor) == Kirigami.ColorUtils.Light)
            return Qt.darker(Kirigami.Theme.backgroundColor, 1.1)
        else
            return Qt.lighter(Kirigami.Theme.backgroundColor, 1.3)
    }

    Kirigami.Theme.colorSet: {
        return Kirigami.Theme.Button
        // if (Array.from(messagesSelectionModel.selectedIndexes).includes(modelIndex)) {
        //     return Kirigami.Theme.Selection
        // }
        // return messagesRoute.model.userID() == authorID ? Kirigami.Theme.Button : Kirigami.Theme.Window
    }
    Kirigami.Theme.inherit: false

    GlobalComponents.ResponsiveMenu {
        id: __responsiveMenu
        GlobalComponents.ResponsiveMenuItem {
            enabled: chatData.data.mCanSendMessages && del.isOwnMessage && messageData.data.kind === "messageText"
            text: i18nc("popup menu for message", "Edit")
            onTriggered: {
                messagesViewRoot.interactionID = del.mID
                messagesViewRoot.interactionKind = "edit"
                del.edit()
            }
        }
        GlobalComponents.ResponsiveMenuItem {
            enabled: chatData.data.mCanSendMessages
            text: i18nc("popup menu for message", "Reply")
            onTriggered: {
                messagesViewRoot.interactionID = del.mID
                messagesViewRoot.interactionKind = "reply"
            }
        }
        GlobalComponents.ResponsiveMenuItem {
            enabled: del.canDeleteMessage
            text: i18nc("popup menu for message", "Delete")
            onTriggered: {
                deleteDialog.chatID = del.mChatID
                deleteDialog.messageID = del.mID
                deleteDialog.canDeleteForSelf = messageData.data.canDeleteForSelf
                deleteDialog.canDeleteForOthers = messageData.data.canDeleteForOthers
                deleteDialog.title = chatData.data.mTitle
                deleteDialog.open()
            }
        }
    }

    background: Item {
        Kirigami.Separator {
            visible: del.separateFromPrevious && settings.thinMode
            opacity: 0.5
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
        }
    }

    contentItem: RowLayout {
        Kirigami.Avatar {
            name: userData.data.name
            source: userData.data.smallAvatar

            implicitWidth: Kirigami.Units.gridUnit*2
            implicitHeight: Kirigami.Units.gridUnit*2

            // visible yoinks from layout, which isn't what we want.
            opacity: del.showAvatar ? 1 : 0

            visible: !settings.thinMode && !chatData.data.mIsChannel

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    globalUserDataSheet.userID = messageData.data.authorID
                    globalUserDataSheet.open()
                }
            }

            layer.enabled: true
            layer.effect: DropShadow {
                cached: true
                horizontalOffset: 0
                verticalOffset: 0
                radius: 4.0
                samples: 17
                color: "#30000000"
            }

            Layout.alignment: Qt.AlignBottom
            Layout.leftMargin: Kirigami.Units.largeSpacing
        }

        Item {
            Layout.fillWidth: (del.isOwnMessage && Kirigami.Settings.isMobile)
        }

        GlobalComponents.LoaderSwitch {
            id: __loaderSwitch

            value: messageData.data.kind
            cases: {
                "messageText": Qt.resolvedUrl((settings.thinMode ? "thin/" : "normal/") + "TextMessage.qml"),
                "messagePhoto": Qt.resolvedUrl((settings.thinMode ? "thin/" : "normal/") + "PhotoMessage.qml"),
                "messageDocument": Qt.resolvedUrl((settings.thinMode ? "thin/" : "normal/") + "FileMessage.qml"),
                "messageChatAddMembers": Qt.resolvedUrl((settings.thinMode ? "thin/" : "normal/") + "UserAddMessage.qml"),
                "messageSticker": Qt.resolvedUrl((settings.thinMode ? "thin/" : "normal/") + "Sticker.qml"),
                "messageAudio": Qt.resolvedUrl((settings.thinMode ? "thin/" : "normal/") + "AudioMessage.qml"),
                "messageVideo": Qt.resolvedUrl((settings.thinMode ? "thin/" : "normal/") + "VideoMessage.qml"),
                "messageAnimation": Qt.resolvedUrl((settings.thinMode ? "thin/" : "normal/") + "GifMessage.qml"),
                "messageVoiceChatStarted": Qt.resolvedUrl((settings.thinMode ? "thin/" : "normal/") + "VoiceChatStarted.qml"),
                "messageVoiceChatEnded": Qt.resolvedUrl((settings.thinMode ? "thin/" : "normal/") + "VoiceChatFinished.qml"),
            }
            defaultCase: Qt.resolvedUrl("Unsupported.qml")

            TapHandler {
                acceptedButtons: Qt.RightButton
                onTapped: __responsiveMenu.open()
            }
            TapHandler {
                onLongPressed: __responsiveMenu.open()
            }
            HoverHandler {
                id: baseH
            }

            ColumnLayout {
                z: 1000
                anchors.left: parent.right
                anchors.top: parent.top

                HoverHandler {
                    id: rowH
                }

                visible: baseH.hovered || rowH.hovered

                QQC2.Button {
                    enabled: chatData.data.mCanSendMessages && del.isOwnMessage && messageData.data.kind === "messageText"
                    QQC2.ToolTip.visible: hovered
                    QQC2.ToolTip.text: i18nc("button", "Edit")
                    icon.name: "document-edit"
                    onClicked: {
                        messagesViewRoot.interactionID = del.mID
                        messagesViewRoot.interactionKind = "edit"
                        del.edit()
                    }
                }
                QQC2.Button {
                    enabled: chatData.data.mCanSendMessages
                    QQC2.ToolTip.visible: hovered
                    QQC2.ToolTip.text: i18nc("button", "Reply")
                    icon.name: "format-text-blockquote"
                    onClicked: {
                        messagesViewRoot.interactionID = del.mID
                        messagesViewRoot.interactionKind = "reply"
                    }
                }
                QQC2.Button {
                    enabled: del.canDeleteMessage
                    QQC2.ToolTip.visible: hovered
                    QQC2.ToolTip.text: i18nc("button", "Delete")
                    icon.name: "edit-delete"
                    onClicked: {
                        deleteDialog.chatID = del.mChatID
                        deleteDialog.messageID = del.mID
                        deleteDialog.canDeleteForSelf = messageData.data.canDeleteForSelf
                        deleteDialog.canDeleteForOthers = messageData.data.canDeleteForOthers
                        deleteDialog.title = chatData.data.mTitle
                        deleteDialog.open()
                    }
                }
            }
        }

        Item {
            Layout.fillWidth: !(del.isOwnMessage && Kirigami.Settings.isMobile)
        }
    }

    width: parent && parent.width > 0 ? parent.width : implicitWidth
    Layout.fillWidth: true

    Tok.RelationalListener {
        id: messageData

        model: tClient.messagesStore
        key: [del.mChatID, del.mID]
        shape: QtObject {
            required property string authorID
            required property string kind
            required property string timestamp
            required property string editedTimestamp
            required property string sendingState

            required property bool canDeleteForSelf
            required property bool canDeleteForOthers
        }
    }
    Tok.RelationalListener {
        id: previousData

        model: tClient.messagesStore
        key: [del.mChatID, del.mPreviousID]
        shape: QtObject {
            required property string authorID
        }
    }
    Tok.RelationalListener {
        id: nextData

        model: tClient.messagesStore
        key: [del.mChatID, del.mNextID]
        shape: QtObject {
            required property string authorID
        }
    }
    Tok.RelationalListener {
        id: userData

        model: tClient.userDataModel
        key: messageData.data.authorID
        shape: QtObject {
            required property string name
            required property string smallAvatar
        }
    }
}