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
        title: i18nc("menu", "File")

        CreateNewMenuInWindow {
        }
        // QQC2.MenuItem {
        //     text: i18nc("menu", "Chat With…")
        // }
        QQC2.MenuSeparator { }
        QQC2.MenuItem {
            text: i18nc("menu", "Log Out")
            onTriggered: tClient.logOut()
        }
        QQC2.MenuItem {
            text: i18nc("menu", "Quit")
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
