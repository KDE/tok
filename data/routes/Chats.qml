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

Kirigami.ScrollablePage {
    header: Components.Header {
        RowLayout {
            Layout.margins: Kirigami.Units.largeSpacing
            Layout.leftMargin: !rootRow.shouldUseSidebars ? rootRow.leftOffset+Kirigami.Units.largeSpacing : Kirigami.Units.largeSpacing
            Layout.rightMargin: !rootRow.shouldUseSidebars ? rootRow.rightOffset+Kirigami.Units.largeSpacing : Kirigami.Units.largeSpacing

            Layout.fillWidth: true

            QQC2.ToolButton {
                QQC2.Menu {
                    id: settingsMenu

                    QQC2.MenuItem {
                        text: i18nc("Checkable menu item to toggle between the default appearance and a more compact style", "Compact mode")
                        checkable: true
                        onToggled: settings.thinMode = checked
                        checked: settings.thinMode
                    }
                    QQC2.MenuItem {
                        text: i18nc("Checkable menu item to toggle between a photo background for the chat and using a solid colour", "Image background")
                        checkable: true
                        onToggled: settings.imageBackground = checked
                        checked: settings.imageBackground
                    }
                    QQC2.MenuItem {
                        text: i18nc("Checkable menu item to toggle viewing content underneath the window (transparent window bg)", "Window transparency")
                        checkable: true
                        onToggled: settings.transparent = checked
                        checked: settings.transparent
                    }
                    QQC2.MenuItem {
                        text: i18nc("menu item that opens a UI element called the 'Quick Switcher', which offers a fast keyboard-based interface for switching in between chats.", "Quick Switcher")
                        onTriggered: quickView.open()
                    }
                    QQC2.MenuItem {
                        text: i18n("Log out")
                        onTriggered: tClient.logOut()
                    }
                    QQC2.Menu {
                        title: i18nc("menu item that has a submenu listing a bunch of colour schemes users can pick from", "Color schemes")
                        Repeater {
                            model: Tok.ColorSchemer.model
                            QQC2.MenuItem {
                                required property int index
                                required property string colorSchemeName

                                text: colorSchemeName

                                onClicked: Tok.ColorSchemer.apply(index)
                            }
                        }
                    }
                }
                icon.name: "application-menu"
                onClicked: settingsMenu.popup()
            }

            Kirigami.SearchField {
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
        onActivated: {
            lView.decrementCurrentIndex()
            lView.itemAtIndex(lView.currentIndex).clicked()
        }
    }
    Shortcut {
        sequence: "Alt+Down"
        onActivated: {
            lView.incrementCurrentIndex()
            lView.itemAtIndex(lView.currentIndex).clicked()
        }
    }
    Shortcut {
        sequence: "Ctrl+K"
        onActivated: {
            quickView.open()
        }
    }

    ChatsComponents.QuickView { id: quickView }

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
            Kirigami.PageRouter.router.pageStack.currentItem.doit()
        }

        reuseItems: true
        activeFocusOnTab: true

        model: Tok.ChatSortModel {
            sourceModel: tClient.chatsModel
            store: tClient.chatsStore
            filter: searchField.text
        }

        header: Item {
            height: !rootRow.shouldUseSidebars && audioBar.active ? audioBar.item.height : 0
        }

        delegate: Kirigami.BasicListItem {
            id: del

            required property string mID

            Tok.RelationalListener {
                id: chatData

                model: tClient.chatsStore
                key: del.mID
                shape: QtObject {
                    required property string mTitle
                    required property string mLastMessageID
                    required property string mPhoto
                    required property int    mUnreadCount
                }
            }

            topPadding: Kirigami.Units.largeSpacing
            bottomPadding: Kirigami.Units.largeSpacing

            text: chatData.data.mTitle
            subtitle: `${plaintext.hasAuthor ? plaintext.authorName + ": " : ""}${plaintext.onelinePlaintext}`

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

            Components.PlaintextMessage {
                id: plaintext

                messagesModel: tClient.messagesStore
                userModel: tClient.userDataModel

                chatID: del.mID
                messageID: chatData.data.mLastMessageID
            }

            checked: Kirigami.PageRouter.router.params.chatID === del.mID
            checkable: Kirigami.PageRouter.router.params.chatID === del.mID
            highlighted: false

            leading: Kirigami.Avatar {
                name: chatData.data.mTitle
                source: chatData.data.mPhoto

                width: height
            }
            trailing: RowLayout {
                QQC2.Label {
                    text: chatData.data.mUnreadCount
                    visible: chatData.data.mUnreadCount > 0
                    padding: Kirigami.Units.smallSpacing

                    horizontalAlignment: Qt.AlignHCenter

                    Layout.minimumWidth: implicitHeight

                    background: Rectangle {
                        color: Kirigami.Theme.focusColor
                        radius: height / 2
                    }
                }
            }

            onClicked: lView.triggerPage(del.mID)
        }

        Kirigami.PlaceholderMessage {
            text: i18n("No chats available.")

            anchors.centerIn: parent

            visible: parent.count === 0
        }
    }

    // global stuff that doesn't need to be reconstructed per view or delegate
    property Item whatever: Item {
        QQC2.Popup {
            id: deleteDialog

            modal: true
            parent: QQC2.Overlay.overlay
            x: (QQC2.Overlay.overlay.width / 2) - (this.width / 2)
            y: (QQC2.Overlay.overlay.height / 2) - (this.height / 2)

            property string chatID
            property string messageID
            property bool canDeleteForSelf: false
            property bool canDeleteForOthers: false

            padding: Kirigami.Units.gridUnit

            contentItem: ColumnLayout {
                Kirigami.Heading {
                    text: i18n("Do you want to delete this message?")
                    level: 4

                    Layout.bottomMargin: Kirigami.Units.gridUnit
                }
                QQC2.Label {
                    visible: deleteDialog.canDeleteForSelf && !deleteDialog.canDeleteForOthers
                    text: i18n("This will delete it just for you.")
                }
                QQC2.Label {
                    visible: !deleteDialog.canDeleteForSelf && deleteDialog.canDeleteForOthers
                    text: i18n("This will delete it for everyone in this chat.")
                }
                QQC2.CheckBox {
                    id: deleteForAll

                    checked: true
                    text: chatData.key[0] == "-" ? i18n("Also delete for everyone") : i18n("Also delete for %1", chatData.data.mTitle)
                    visible: deleteDialog.canDeleteForSelf && deleteDialog.canDeleteForOthers
                }
                // QQC2.CheckBox {
                //     text: i18n("Report Spam")
                // }
                // QQC2.CheckBox {
                //     text: i18n("Delete all from this user")
                // }
                RowLayout {
                    Layout.topMargin: Kirigami.Units.gridUnit

                    Item { Layout.fillWidth: true }
                    QQC2.Button {
                        text: i18n("Cancel")
                        onClicked: deleteDialog.close()
                    }
                    QQC2.Button {
                        text: i18n("Delete")
                        onClicked: {
                            tClient.messagesStore.deleteMessage(deleteDialog.chatID, deleteDialog.messageID, deleteForAll.checked)
                            deleteDialog.close()
                        }
                    }
                }
            }
        }
    }
}

}
