// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

import "qrc:/components" as GlobalComponents

QQC2.Control {
    id: del

    required property string mID
    required property string mChatID
    required property string mNextID
    required property string mPreviousID

    readonly property int recommendedSize: (rootRow.wideMode ? Math.max(del.width / 3, Kirigami.Units.gridUnit * 15) : (del.width * 0.8))

    readonly property bool isOwnMessage: messageData.data.authorID === tClient.ownID
    readonly property bool showAvatar: !serviceMessage && (nextData.data.authorID != messageData.data.authorID) && (!(Kirigami.Settings.isMobile && isOwnMessage))
    readonly property bool serviceMessage: {
        return messageData.data.kind == "messageChatAddMembers"
    }
    readonly property bool separateFromPrevious: previousData.data.authorID != messageData.data.authorID
    readonly property bool canDeleteMessage: messageData.data.canDeleteForSelf || messageData.data.canDeleteForOthers

    topPadding: settings.slimMode ? (del.separateFromPrevious ? Kirigami.Units.smallSpacing : 0) : (del.separateFromPrevious ? Kirigami.Units.largeSpacing : Kirigami.Units.smallSpacing)
    bottomPadding: 0

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
        GlobalComponents.ResponsiveMenuItem {
            enabled: chatData.data.mCanSendMessages
            text: i18nc("popup menu for message", "Reply")
            onTriggered: messagesViewRoot.replyToID = del.mID
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

            visible: !settings.thinMode

            HoverHandler {
                cursorShape: Qt.PointingHandCursor
            }
            TapHandler {
                onTapped: __userDataSheet.open()
            }

            GlobalComponents.UserDataSheet { id: __userDataSheet; userID: messageData.data.authorID || "" }

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
                "messageText": Qt.resolvedUrl((settings.thinMode ? "thin/" : "") + "TextMessage.qml"),
                "messagePhoto": Qt.resolvedUrl((settings.thinMode ? "thin/" : "") + "PhotoMessage.qml"),
                "messageDocument": Qt.resolvedUrl((settings.thinMode ? "thin/" : "") + "FileMessage.qml"),
                "messageChatAddMembers": Qt.resolvedUrl((settings.thinMode ? "thin/" : "") + "UserAddMessage.qml"),
                "messageSticker": Qt.resolvedUrl((settings.thinMode ? "thin/" : "") + "Sticker.qml"),
                "messageAudio": Qt.resolvedUrl((settings.thinMode ? "thin/" : "") + "AudioMessage.qml"),
                "messageVideo": Qt.resolvedUrl((settings.thinMode ? "thin/" : "") + "VideoMessage.qml"),
                "messageAnimation": Qt.resolvedUrl((settings.thinMode ? "thin/" : "") + "GifMessage.qml"),
            }
            defaultCase: Qt.resolvedUrl("Unsupported.qml")

            TapHandler {
                acceptedButtons: Qt.RightButton
                onTapped: __responsiveMenu.open()
            }
            TapHandler {
                onLongPressed: __responsiveMenu.open()
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