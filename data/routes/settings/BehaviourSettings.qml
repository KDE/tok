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
            text: i18nc("Checkable control to toggle a system tray icon and not closing the app when X is pressed, instead using the tray icon", "Minimize to system tray when I close the app")
            checkable: true
            onToggled: settings.userWantsSystemTray = checked
            checked: settings.userWantsSystemTray
        }

        QQC2.CheckBox {
            text: i18nc("Checkable control to toggle a feature that makes the user able to interact with Telegram without others knowing they are doing so; like a ghost, hence the metaphor. Translate using a metaphor that makes sense, not literally", "Enable Ghost Mode")
            checkable: true
            onToggled: settings.ghostMode = checked
            checked: settings.ghostMode
        }

        QQC2.Label {
            text: i18nc("Explanation for the Ghost Mode checkbox", "Ghost Mode allows you to read messages without telling Telegram or other users that you're reading messages. Note that this means that the unread message counter does not change when you read messages in Ghost Mode.")
            wrapMode: Text.Wrap
            width: parent.width
        }
    }

    Layout.fillHeight: true
    Layout.fillWidth: true
}
