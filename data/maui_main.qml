// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.10
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.10 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.mauikit.controls 1.2 as Maui

import "qrc:/components" as Components

Maui.ApplicationWindow {
    id: rootWindow

    property var aplayer: Components.AudioPlayer

    title: i18nc("window title", "Tok")

    width: Kirigami.Units.gridUnit * 65
    height: Kirigami.Units.gridUnit * 35

    color: cont.settings.transparent ? "transparent" : Kirigami.Theme.backgroundColor
    flags: content.settings.transparent ? Qt.WA_TranslucentBackground : 0
    background.visible: false

    header: Item {}

    Maui.App.enableCSD: true

    onClosing: (e) => {
        cont.closing(e)
    }

    Content {
        id: cont

        anchors.fill: parent

        leftOffset: lhControls.width>0 ? lhControls.width+lhControls.anchors.leftMargin : 0
        rightOffset: rhControls.width>0 ? rhControls.width+rhControls.anchors.rightMargin : 0
    }
    Components.Header {
        id: dummyHeader
        visible: false
    }
    Maui.WindowControls {
        id: lhControls

        parent: QQC2.Overlay.overlay
        side: Qt.LeftEdge
        z: 300

        anchors.left: parent.left
        anchors.leftMargin: y

        y: Math.round(dummyHeader.height/2 - height/2)
    }
    Maui.WindowControls {
        id: rhControls

        parent: QQC2.Overlay.overlay
        side: Qt.RightEdge
        z: 300

        anchors.right: parent.right
        anchors.rightMargin: y

        y: Math.round(dummyHeader.height/2 - height/2)
    }
}
