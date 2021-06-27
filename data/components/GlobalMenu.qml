// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import Qt.labs.platform 1.1 as Labs
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.10
import org.kde.Tok 1.0 as Tok
import org.kde.kirigami 2.15 as Kirigami

Labs.MenuBar {
    Labs.Menu {
        title: i18nc("menu", "File")

        Labs.MenuItem {
            text: i18nc("menu", "Log Out")
            onTriggered: tClient.logOut()
        }
        Labs.MenuItem {
            text: i18nc("menu", "Quit")
        }
    }
    EditMenu {
        title: i18nc("menu", "Edit")
        field: (rootWindow.activeFocusItem instanceof TextEdit || rootWindow.activeFocusItem instanceof TextInput) ? rootWindow.activeFocusItem : null
        showFormat: false
    }
    Labs.Menu {
        title: i18nc("menu", "View")

        Labs.MenuItem {
            text: settings.thinMode ? i18nc("menu", "Disable Compact Mode") : i18nc("menu", "Enable Compact Mode")
            onTriggered: settings.thinMode = !settings.thinMode
        }
        Labs.MenuItem {
            text: settings.imageBackground ? i18nc("menu", "Don't Use Image As Background") : i18nc("menu", "Use Image As Background")
            onTriggered: settings.imageBackground = !settings.imageBackground
        }
        Labs.MenuItem {
            text: settings.transparent ? i18nc("menu", "Disable Transparency") : i18nc("menu", "Enable Transparency")
            onTriggered: settings.transparent = !settings.transparent
        }
        Labs.MenuItem {
            text: i18nc("menu item that opens a UI element called the 'Quick Switcher', which offers a fast keyboard-based interface for switching in between chats.", "Open Quick Switcher")
            onTriggered: quickView.open()
        }
    }
    FormatMenu {
        title: i18nc("menu", "Format")
        field: rootWindow.activeFocusItem instanceof TextEdit ? rootWindow.activeFocusItem : null
    }
    Labs.Menu {
        title: i18nc("menu", "Window")

        Labs.MenuItem {
            text: settings.userWantsSidebars ? i18nc("menu", "Hide Sidebar") : i18nc("menu", "Show Sidebar")
            onTriggered: settings.userWantsSidebars = !settings.userWantsSidebars
        }
        Labs.MenuItem {
            text: rootWindow.visibility === Window.FullScreen ? i18nc("menu", "Exit Full Screen") : i18nc("menu", "Enter Full Screen")
            onTriggered: rootWindow.visibility === Window.FullScreen ? rootWindow.showNormal() : rootWindow.showFullScreen()
        }
    }
    // TODO: offline help system
    Labs.Menu {
        title: i18nc("menu", "Help")

        Labs.MenuItem {
            text: i18nc("menu", "Telegram FAQ")
            onTriggered: Qt.openUrlExternally("https://telegram.org/faq")
        }
        // TODO: implement the necessary infrastructure for this
        // Labs.MenuItem {
        //     text: i18nc("ask a question; contacts a live, actual human who will initiate a DM", "Ask A Question…")
        // }
    }
}
