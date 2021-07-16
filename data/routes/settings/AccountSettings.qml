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

        QQC2.Button {
            text: i18n("Log out")
            onClicked: tClient.logOut()
        }
    }

    Layout.fillHeight: true
    Layout.fillWidth: true
}
