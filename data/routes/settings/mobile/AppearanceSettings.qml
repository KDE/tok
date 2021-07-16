// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.kitemmodels 1.0
import org.kde.Tok 1.0 as Tok

Kirigami.ScrollablePage {
    topPadding: 0
    leftPadding: 0
    bottomPadding: 0
    rightPadding: 0

    titleDelegate: RowLayout {
        QQC2.ToolButton {
            icon.name: "arrow-left"
            onClicked: rootRow.layers.pop()
        }
        Kirigami.Heading {
            text: i18nc("title", "Appearance")
            level: 4
        }
        Item { Layout.fillWidth: true }
    }

    ColumnLayout {
        spacing: 0

        Kirigami.Icon {
            source: "preferences-desktop-color"

            Layout.preferredHeight: Kirigami.Units.iconSizes.huge
            Layout.preferredWidth: Kirigami.Units.iconSizes.huge
            Layout.margins: Kirigami.Units.gridUnit

            Layout.alignment: Qt.AlignHCenter
        }

        Kirigami.BasicListItem {
            text: i18nc("Checkable control to toggle between the default appearance and a more compact style. Keep the text no longer than a few letters longer than the source string due to screen size constraints. Reword heavily if needed.", "Compact appearance for messages")
            onClicked: settings.thinMode = !settings.thinMode

            trailing: QQC2.CheckBox {
                checked: settings.thinMode
                enabled: false
            }
        }
        Kirigami.BasicListItem {
            text: i18nc("Checkable control to toggle between a photo background for the chat and using a solid colour. Keep the text no longer than a few letters longer than the source string due to screen size constraints. Reword heavily if needed.", "Show image behind chats")
            onClicked: settings.imageBackground = !settings.imageBackground

            trailing: QQC2.CheckBox {
                checked: settings.imageBackground
                enabled: false
            }
        }
        Kirigami.BasicListItem {
            text: i18nc("List item that opens a page to change the colour scheme", "Change color scheme")
            onClicked: rootRow.layers.push(Qt.resolvedUrl("ColorSchemes.qml"))

            trailing: Item {
                Kirigami.Icon {
                    source: "arrow-right"
                    height: Kirigami.Units.iconSizes.small
                    width: height

                    anchors {
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
    }
}