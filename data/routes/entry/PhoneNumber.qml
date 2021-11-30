// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.10
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.12 as Kirigami

import "qrc:/components" as Components

Kirigami.PageRoute {

name: "Entry/PhoneNumber"

Kirigami.Page {
    header: Components.Header {
        Kirigami.Heading {
            text: i18n("Welcome")

            Layout.margins: Kirigami.Units.largeSpacing
            Layout.fillWidth: true
        }
    }

    Kirigami.Theme.colorSet: Kirigami.Theme.View

    ColumnLayout {
        anchors.centerIn: parent
        width: Math.min(parent.width, implicitWidth)

        Image {
            source: "qrc:/img/org.kde.Tok.svg"
            sourceSize {
                width: Layout.preferredWidth
                height: Layout.preferredWidth
            }

            Layout.preferredWidth: Kirigami.Units.gridUnit * 7
            Layout.preferredHeight: Layout.preferredWidth

            Layout.alignment: Qt.AlignHCenter
        }

        Kirigami.Heading {
            text: qsTr("Welcome to Tok")
            horizontalAlignment: Text.AlignHCenter

            Layout.fillWidth: true
        }
        Kirigami.Heading {
            text: qsTr("Enter the phone number for your Telegram account to continue")
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            level: 4
            opacity: 0.7

            Layout.fillWidth: true
        }

        Item { implicitHeight: Kirigami.Units.largeSpacing }

        RowLayout {
            QQC2.TextField {
                id: field

                placeholderText: i18n("Phone number")
                onAccepted: tClient.enterPhoneNumber(text)

                Component.onCompleted: this.forceActiveFocus()
            }
            QQC2.Button {
                icon.name: Qt.application.layoutDirection == Qt.LeftToRight ? "arrow-right" : "arrow-left"
                onClicked: field.accepted()

                Accessible.name: i18n("Continue")
                QQC2.ToolTip.text: i18n("Continue")
                QQC2.ToolTip.visible: hovered
            }
            Layout.alignment: Qt.AlignHCenter
        }

        Item { implicitHeight: Kirigami.Units.largeSpacing }

        QQC2.Button {
            text: i18n("Configure Proxy…")
            visible: Kirigami.Settings.isMobile

            onClicked: rootRow.layers.push(Qt.resolvedUrl("qrc:/routes/settings/mobile/ProxySettings.qml"))

            Layout.alignment: Qt.AlignHCenter
        }
    }
}

}
