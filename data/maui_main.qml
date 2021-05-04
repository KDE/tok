import QtQuick 2.10
import QtQuick.Layouts 1.10
import org.kde.kirigami 2.14 as Kirigami
import org.mauikit.controls 1.2 as Maui

import "qrc:/components" as Components

Maui.ApplicationWindow {
    id: rootWindow

    width: 800
    height: 600

    header: Item {}

    Maui.App.enableCSD: true

    Content {
        id: cont

        anchors.fill: parent
    }
    Components.Header {
        id: dummyHeader
        visible: false
    }
    Maui.WindowControls {
        side: Qt.LeftEdge

        anchors.left: parent.left
        anchors.leftMargin: y

        y: Math.round(dummyHeader.height/2 - height/2)
    }
    Maui.WindowControls {
        side: Qt.RightEdge

        anchors.right: parent.right
        anchors.rightMargin: y

        y: Math.round(dummyHeader.height/2 - height/2)
    }
}
