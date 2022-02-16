// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.kitemmodels 1.0
import org.kde.Tok 1.0 as Tok
import "qrc:/components" as Components
import "qrc:/routes/chats components" as ChatsComponents

Kirigami.PageRoute {

name: "Chats"

Kirigami.Page {
    topPadding: 0
    leftPadding: 0
    rightPadding: 0
    bottomPadding: 0

    header: Components.Header {
        RowLayout {
            Layout.margins: Kirigami.Units.largeSpacing
            Layout.leftMargin: !rootRow.shouldUseSidebars ? rootRow.leftOffset+Kirigami.Units.largeSpacing : Kirigami.Units.largeSpacing
            Layout.rightMargin: !rootRow.shouldUseSidebars ? rootRow.rightOffset+Kirigami.Units.largeSpacing : Kirigami.Units.largeSpacing

            Layout.fillWidth: true

            QQC2.ToolButton {
                icon.name: "settings-configure"
                visible: Kirigami.Settings.isMobile
                onClicked: rootRow.layers.push(Qt.resolvedUrl("qrc:/routes/settings/mobile/Settings.qml"))
            }

            Components.SearchField {
                id: searchField

                Layout.fillWidth: true
                Keys.onTabPressed: nextItemInFocusChain().forceActiveFocus(Qt.TabFocusReason)

                Accessible.name: i18n("Search chats")
                Accessible.description: ``
                Accessible.searchEdit: true
            }
        }
    }
    activeFocusOnTab: true

    // yes, the decrement goes up and the increment goes down visually.
    Shortcut {
        sequence: "Alt+Up"
        context: Qt.ApplicationShortcut
        onActivated: {
            lView.decrementCurrentIndex()
            lView.itemAtIndex(lView.currentIndex).clicked()
        }
    }
    Shortcut {
        sequence: "Alt+Down"
        context: Qt.ApplicationShortcut
        onActivated: {
            lView.incrementCurrentIndex()
            lView.itemAtIndex(lView.currentIndex).clicked()
        }
    }
    Shortcut {
        sequence: "Ctrl+K"
        context: Qt.ApplicationShortcut
        onActivated: {
            quickView.open()
        }
    }

    ChatsComponents.QuickView { id: quickView }

    ColumnLayout {

    anchors.fill: parent

    Item {
        implicitHeight: !rootRow.shouldUseSidebars && audioBar.active ? audioBar.item.height : 0
    }

    RowLayout {

    spacing: 0

    QQC2.ScrollView {
        visible: !Kirigami.Settings.isMobile && sidebarView.count > 1
        contentWidth: Kirigami.Units.gridUnit*4
        Layout.fillHeight: true

        QQC2.ScrollBar.horizontal.policy: QQC2.ScrollBar.AlwaysOff

        ListView {
            id: sidebarView

            width: Kirigami.Units.gridUnit*4
            model: tClient.chatListModel
            delegate: QQC2.ToolButton {
                required property string name
                required property string chatListID

                height: Kirigami.Units.gridUnit*4
                width: Kirigami.Units.gridUnit*4

                text: name
                checked: filter.folder == chatListID

                onClicked: filter.folder = chatListID
            }
        }
    }

    QQC2.ScrollView {

    QQC2.ScrollBar.horizontal.policy: QQC2.ScrollBar.AlwaysOff

    Layout.fillHeight: true
    Layout.fillWidth: true

    ListView {
        id: lView

        function triggerPage(chatID) {
            if (Kirigami.PageRouter.router.params.chatID === chatID) {
                Kirigami.PageRouter.bringToView(1)
                Kirigami.PageRouter.router.pageStack.currentItem.doit()
                return
            }
            if (Kirigami.PageRouter.router.params.chatID !== undefined) {
                tClient.messagesModel(Kirigami.PageRouter.router.params.chatID).comingOut()
            }
            tClient.messagesModel(chatID).comingIn()
            Kirigami.PageRouter.pushFromHere({ "route": "Messages/View", "chatID": chatID })
            tryit(() => Kirigami.PageRouter.router.pageStack.currentItem.doit())
        }

        reuseItems: true
        activeFocusOnTab: true

        model: Tok.ChatSortModel {
            id: filter

            sourceModel: tClient.chatsModel
            store: tClient.chatsStore
            filter: searchField.text
        }

        Component {
            id: mobileTabs
            QQC2.ScrollView {
                visible: mobileChatListView.count > 1

                ListView {
                    id: mobileChatListView
                    model: tClient.chatListModel
                    delegate: QQC2.TabButton {
                        required property string name
                        required property string chatListID

                        text: name
                        checked: filter.folder == chatListID

                        onClicked: filter.folder = chatListID
                    }
                    orientation: ListView.Horizontal
                }

                contentHeight: 30
                QQC2.ScrollBar.vertical.policy: QQC2.ScrollBar.AlwaysOff
                Layout.fillWidth: true
            }
        }

        header: Kirigami.Settings.isMobile ? mobileTabs : null
        footer: RowLayout {
            z: 100
            width: parent.width

            Item { Layout.fillWidth: true }
            Components.HopToEdge {
                isDown: false
                view: lView

                Layout.margins: Kirigami.Units.gridUnit
            }
            Components.CreateNewMenu {
                id: createNew
            }
            QQC2.RoundButton {
                visible: Kirigami.Settings.isMobile
                icon.name: "list-add"
                text: i18nc("button with menu", "Create New...")
                onClicked: createNew.open()

                Layout.margins: Kirigami.Units.gridUnit
            }
        }
        footerPositioning: ListView.OverlayFooter

        delegate: QQC2.ItemDelegate {
            id: del

            required property string mID

            width: (parent || {width: 0}).width

            topPadding: Kirigami.Units.largeSpacing
            bottomPadding: Kirigami.Units.largeSpacing

            onClicked: lView.triggerPage(del.mID)

            Tok.RelationalListener {
                id: chatData

                model: tClient.chatsStore
                key: del.mID
                shape: QtObject {
                    required property string mTitle
                    required property string mLastMessageID
                    required property string mPhoto
                    required property string mKind
                    required property bool   mIsSaved
                    required property var    mCurrentActions
                    required property int    mUnreadCount
                }
            }

            Components.PlaintextMessage {
                id: plaintext

                messagesModel: tClient.messagesStore
                userModel: tClient.userDataModel

                chatID: del.mID
                messageID: chatData.data.mLastMessageID
            }

            Accessible.name: {
                let strings = [`${chatData.data.mTitle}.`]
                if (chatData.data.mUnreadCount > 0) {
                    strings.push(i18np("1 unread message.", "%1 unread messages.", chatData.data.mUnreadCount))
                }
                if (plaintext.hasAuthor) {
                    strings.push(i18nc("%1 is the message author (probably, but not necessarily a human), and %2 is the message contents", "Latest message from %1: %2", plaintext.authorName, plaintext.onelinePlaintext))
                } else {
                    strings.push(i18nc("%1 is the message contents", "Latest message: %1", plaintext.onelinePlaintext))
                }
                return strings.join(" ")
            }

            background: Rectangle {
                readonly property color bgColor: {
                    if (Kirigami.PageRouter.router.params.chatID === del.mID)
                        return Kirigami.Theme.highlightColor

                    if (del.hovered || del.visualFocus)
                        return Kirigami.ColorUtils.adjustColor(Kirigami.Theme.highlightColor, {alpha: 15})

                    return Kirigami.Theme.backgroundColor
                }
                color: Kirigami.ColorUtils.adjustColor(bgColor, {alpha: 80})
                border.color: bgColor
                border.width: 1
                radius: 3

                Kirigami.Separator {
                    visible: Kirigami.PageRouter.router.params.chatID !== del.mID
                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                        leftMargin: Kirigami.Units.largeSpacing
                        rightMargin: Kirigami.Units.largeSpacing
                    }
                    weight: Kirigami.Separator.Weight.Light
                }
            }

            contentItem: RowLayout {
                spacing: 6

                Kirigami.Avatar {
                    name: chatData.data.mTitle
                    source: chatData.data.mPhoto
                    imageMode: chatData.data.mIsSaved ?
                        Kirigami.Avatar.AlwaysShowInitials :
                        Kirigami.Avatar.AdaptiveImageOrInitals
                    initialsMode: chatData.data.mIsSaved ?
                        Kirigami.Avatar.UseIcon :
                        Kirigami.Avatar.UseInitials
                    iconSource: "bookmarks"

                    Layout.preferredHeight: Math.round(Kirigami.Units.gridUnit * 2.5)
                    Layout.preferredWidth: Layout.preferredHeight
                }

                ColumnLayout {
                    Layout.fillWidth: true

                    spacing: 4

                    RowLayout {
                        Layout.fillWidth: true

                        spacing: 0

                        Kirigami.Icon {
                            visible: chatData.data.mKind === "secretChat"
                            source: "lock"

                            color: Kirigami.Theme.positiveTextColor

                            Layout.preferredWidth: 16
                            Layout.preferredHeight: 16
                        }

                        QQC2.Label {
                            text: chatData.data.mTitle
                            elide: Text.ElideMiddle
                            color: chatData.data.mKind === "secretChat" ? Kirigami.Theme.positiveTextColor : Kirigami.Theme.textColor

                            Layout.fillWidth: true
                        }

                        Kirigami.Icon {
                            source: {
                                const states = {
                                    "pending": "clock",
                                    "failed": "emblem-error",
                                    "sent": "emblem-ok-symbolic",
                                }
                                return states[plaintext.universalData.data.sendingState]
                            }
                            color: Kirigami.Theme.textColor

                            Layout.preferredWidth: 16
                            Layout.preferredHeight: 16
                            Layout.rightMargin: 4

                            opacity: 0.5

                            visible: plaintext.isOwn
                        }

                        QQC2.Label {
                            opacity: 0.4
                            color: Kirigami.Theme.textColor

                            text: plaintext.timestamp
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        spacing: 2

                        QQC2.Label {
                            id: textLabel

                            text: chatData.data.mCurrentActions.any ? chatData.data.mCurrentActions.message : `${plaintext.hasAuthor ? plaintext.authorName + ": " : ""}${plaintext.onelinePlaintext}`
                            opacity: 0.7
                            elide: Text.ElideRight
                            color: chatData.data.mCurrentActions.any ? Kirigami.Theme.focusColor : Kirigami.Theme.textColor

                            Layout.fillWidth: true
                        }

                        QQC2.Label {
                            text: chatData.data.mUnreadCount
                            visible: chatData.data.mUnreadCount > 0
                            color: Kirigami.Theme.highlightedTextColor

                            leftPadding: 4
                            rightPadding: 4

                            horizontalAlignment: Qt.AlignHCenter
                            fontSizeMode: Text.VerticalFit

                            Layout.maximumHeight: textLabel.implicitHeight
                            Layout.minimumWidth: textLabel.implicitHeight
                            Layout.fillHeight: true

                            background: Rectangle {
                                color: Kirigami.Theme.highlightColor
                                radius: height / 2
                            }
                        }
                    }
                }
            }
        }

        Kirigami.PlaceholderMessage {
            text: i18n("No chats available.")

            anchors.centerIn: parent

            visible: parent.count === 0
        }
    }

    }

    }

    }
}

}
