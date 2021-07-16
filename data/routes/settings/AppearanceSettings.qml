// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.3 as QQC2
import QtQuick.Window 2.2
import org.kde.Tok 1.0 as Tok

import org.kde.kirigami 2.12 as Kirigami

Item {
    Column {
        id: form

        anchors.fill: parent
        spacing: Kirigami.Units.smallSpacing

        QQC2.CheckBox {
            text: i18nc("Checkable control to toggle between the default appearance and a more compact style", "Use a compact appearance for messages")
            checked: settings.thinMode
            onToggled: settings.thinMode = checked
        }
        QQC2.CheckBox {
            text: i18nc("Checkable control to toggle between a photo background for the chat and using a solid colour", "Display an image in the background of chats")
            checkable: true
            onToggled: settings.imageBackground = checked
            checked: settings.imageBackground
            enabled: !settings.transparent
        }
        QQC2.CheckBox {
            text: i18nc("Checkable control to toggle viewing content underneath the window (transparent window bg)", "Make the window transparent")
            checkable: true
            onToggled: settings.transparent = checked
            checked: settings.transparent
        }
        QQC2.Button {
            text: i18nc("Button that offers a menu to change the colour scheme", "Change Color Scheme")
            onClicked: menudo.popup()

            QQC2.Menu {
                id: menudo

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
    }

    Layout.fillHeight: true
    Layout.fillWidth: true
}
