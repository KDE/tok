// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import Qt.labs.platform 1.1 as QQC2
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.10
import org.kde.Tok 1.0 as Tok
import org.kde.kirigami 2.15 as Kirigami

QQC2.MenuBar {
    QQC2.Menu {
        title: i18nc("menu", "Tok")

        QQC2.MenuItem {
            text: i18nc("menu", "Preferences…")
            onTriggered: content.settingsWindow.showNormal()
        }
        QQC2.MenuItem {
            text: tClient.doNotDisturb ? i18nc("menu", "Enable Notifications") : i18nc("menu", "Disable Notifications")
            onTriggered: tClient.doNotDisturb = !tClient.doNotDisturb
        }
        QQC2.MenuSeparator { }
        QQC2.MenuItem {
            text: i18nc("menu", "Log Out")
            onTriggered: tClient.logOut()
        }
        QQC2.MenuItem {
            text: i18nc("menu", "Quit")
            onTriggered: Qt.quit()
        }
    }
    QQC2.Menu {
        title: i18nc("menu", "Chats")

        CreateNewMenuInWindow {
        }

        Sheet {
            id: newPrivateDM

            function doOpen() {
                contactsPicker.model = tClient.newContactsModel()

                this.open()
            }

            contentItem: ColumnLayout {
                spacing: Kirigami.Units.gridUnit

                Kirigami.Heading {
                    text: i18nc("title", "Select someone to chat with")
                }

                QQC2.ScrollView {
                    Layout.preferredHeight: Kirigami.Units.gridUnit * 20
                    Layout.fillWidth: true

                    QQC2.ScrollBar.horizontal.policy: QQC2.ScrollBar.AlwaysOff

                    ContactsPicker {
                        id: contactsPicker
                        isSelectMultiple: false
                        reuseItems: true

                        onUserSelected: (withUser) => {
                            tClient.chatsModel.createPrivateChat(withUser).then((chatID) => {
                                rootRow.router.navigateToRoute(["Chats", { "route": "Messages/View", "chatID": chatID }])
                                newPrivateDM.close()
                            })
                        }
                    }
                }

                RowLayout {
                    Item { Layout.fillWidth: true }
                    QQC2.Button {
                        text: i18n("Cancel")
                        onClicked: newPrivateDM.close()
                    }
                }
            }
        }

        QQC2.MenuItem {
            text: i18nc("menu", "Chat With…")
            onTriggered: newPrivateDM.doOpen()
        }
    }
    EditMenuInWindow {
        title: i18nc("menu", "Edit")
        field: (rootWindow.activeFocusItem instanceof TextEdit || rootWindow.activeFocusItem instanceof TextInput) ? rootWindow.activeFocusItem : null
        showFormat: false
    }
    QQC2.Menu {
        title: i18nc("menu", "View")

        QQC2.MenuItem {
            text: settings.thinMode ? i18nc("menu", "Disable Compact Mode") : i18nc("menu", "Enable Compact Mode")
            onTriggered: settings.thinMode = !settings.thinMode
        }
        QQC2.MenuItem {
            text: settings.imageBackground ? i18nc("menu", "Don't Use Image As Background") : i18nc("menu", "Use Image As Background")
            onTriggered: settings.imageBackground = !settings.imageBackground
        }
        QQC2.MenuItem {
            text: settings.transparent ? i18nc("menu", "Disable Transparency") : i18nc("menu", "Enable Transparency")
            onTriggered: settings.transparent = !settings.transparent
        }
        QQC2.MenuItem {
            text: i18nc("menu item that opens a UI element called the 'Quick Switcher', which offers a fast keyboard-based interface for switching in between chats.", "Open Quick Switcher")
            onTriggered: quickView.open()
        }

        QQC2.Menu {
            id: colourSchemeMenu
            title: i18nc("menu item offering access to more menu items to pick the colour scheme", "Set Color Scheme")

            Instantiator {
                model: Tok.ColorSchemer.model

                onObjectAdded: (idx, obj) => {
                    colourSchemeMenu.insertItem(idx, obj)
                }
                onObjectRemoved: (idx, obj) => {
                    colourSchemeMenu.removeItem(obj)
                }

                delegate: QQC2.MenuItem {
                    required property int index
                    required property string colorSchemeName

                    text: colorSchemeName

                    onTriggered: Tok.ColorSchemer.apply(index)
                }
            }
        }
    }
    FormatMenuInWindow {
        title: i18nc("menu", "Format")
        field: rootWindow.activeFocusItem instanceof TextEdit ? rootWindow.activeFocusItem : null
    }
    QQC2.Menu {
        title: i18nc("menu", "Window")

        QQC2.MenuItem {
            text: settings.userWantsSidebars ? i18nc("menu", "Hide Sidebar") : i18nc("menu", "Show Sidebar")
            onTriggered: settings.userWantsSidebars = !settings.userWantsSidebars
        }
        QQC2.MenuItem {
            text: rootWindow.visibility === Window.FullScreen ? i18nc("menu", "Exit Full Screen") : i18nc("menu", "Enter Full Screen")
            onTriggered: rootWindow.visibility === Window.FullScreen ? rootWindow.showNormal() : rootWindow.showFullScreen()
        }
    }
    // TODO: offline help system
    QQC2.Menu {
        title: i18nc("menu", "Help")

        QQC2.MenuItem {
            text: i18nc("menu", "Telegram FAQ")
            onTriggered: Qt.openUrlExternally("https://telegram.org/faq")
        }
        // TODO: implement the necessary infrastructure for this
        // QQC2.MenuItem {
        //     text: i18nc("ask a question; contacts a live, actual human who will initiate a DM", "Ask A Question…")
        // }
    }
}
