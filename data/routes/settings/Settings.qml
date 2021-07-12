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

Kirigami.ApplicationWindow {
    visible: false

    title: i18nc("window title", "Settings")

    width: Kirigami.Units.gridUnit * 15
    height: Kirigami.Units.gridUnit * 20

    Kirigami.FormLayout {
        id: form

        anchors.fill: parent

        QQC2.CheckBox {
            text: i18nc("Checkable control to toggle between the default appearance and a more compact style", "Compact mode")
            checked: settings.thinMode
            onToggled: settings.thinMode = checked
        }
        QQC2.CheckBox {
            text: i18nc("Checkable control to toggle between a photo background for the chat and using a solid colour", "Image background")
            checkable: true
            onToggled: settings.imageBackground = checked
            checked: settings.imageBackground
        }
        QQC2.CheckBox {
            text: i18nc("Checkable control to toggle viewing content underneath the window (transparent window bg)", "Window transparency")
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
        QQC2.Button {
            text: i18n("Log out")
            onClicked: tClient.logOut()
        }
    }
}