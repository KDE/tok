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
            text: i18nc("title", "Preferences")
            level: 4
        }
        Item { Layout.fillWidth: true }
    }

    ColumnLayout {
        spacing: 0

        Kirigami.Icon {
            source: "preferences"

            Layout.preferredHeight: Kirigami.Units.iconSizes.huge
            Layout.preferredWidth: Kirigami.Units.iconSizes.huge
            Layout.margins: Kirigami.Units.gridUnit

            Layout.alignment: Qt.AlignHCenter
        }

        Kirigami.BasicListItem {
            text: i18n("Appearance")
            onClicked: rootRow.layers.push(Qt.resolvedUrl("AppearanceSettings.qml"))

            Layout.fillWidth: true
        }

        Kirigami.BasicListItem {
            text: i18nc("Checkable control to toggle Ghost Mode. Keep the text no longer than a few letters longer than the source string due to screen size constraints. Reword heavily if needed.", "Enable Ghost Mode")
            subtitle: i18nc("Explanation for the Ghost Mode toggle", "Ghost Mode allows you to read messages without telling Telegram or other users that you're reading messages. Note that this means that unread message counters in the chat list do not change when you read messages in Ghost Mode.")
            subtitleItem.wrapMode: Text.Wrap
            subtitleItem.elide: Text.ElideNone
            onClicked: settings.ghostMode = !settings.ghostMode

            trailing: QQC2.CheckBox {
                checked: settings.ghostMode
                enabled: false
            }
        }

        Item { Layout.preferredHeight: Kirigami.Units.gridUnit * 2 }

        Kirigami.BasicListItem {
            text: i18n("Log Out")
        }
    }
}