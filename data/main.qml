// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.10
import QtQuick.Controls 2.10 as QQC2
import org.kde.kirigami 2.12 as Kirigami

import "routes" as Routes
import "routes/entry" as EntryRoutes
import "routes/messages" as MessagesRoutes

import "qrc:/components" as Components

Kirigami.AbstractApplicationWindow {
    id: rootWindow

    property var aplayer: Components.AudioPlayer

    width: Kirigami.Units.gridUnit * 65
    height: Kirigami.Units.gridUnit * 35

    title: i18nc("window title", "Tok")

    menuBar: Loader {
        active: Kirigami.Settings.hasPlatformMenuBar != undefined ?
                !Kirigami.Settings.hasPlatformMenuBar && !Kirigami.Settings.isMobile :
                !Kirigami.Settings.isMobile

        sourceComponent: Components.GlobalMenuInWindow {
        }
    }
    property alias settings: content.settings
    property alias rootRow: content

    color: content.settings.transparent ? "transparent" : Kirigami.Theme.backgroundColor
    flags: content.settings.transparent ? Qt.WA_TranslucentBackground : 0

    onClosing: (e) => {
        content.closing(e)
    }

    Content { id: content; anchors.fill: parent }

}
