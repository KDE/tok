// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.12 as Kirigami

Kirigami.PageRoute {

name: "Entry/Welcome"

Kirigami.ScrollablePage {
    ColumnLayout {
        Kirigami.FormLayout {
            Kirigami.Heading {
                text: i18n("Welcome to Tok")
            }

            QQC2.Button {
                text: i18n("Get Started")

                onClicked: Kirigami.PageRouter.navigateToRoute("Entry/PhoneNumber")
            }

            QQC2.Button {
                text: i18n("Configure Proxy…")
                visible: Kirigami.Settings.isMobile

                onClicked: rootRow.layers.push(Qt.resolvedUrl("qrc:/routes/settings/mobile/ProxySettings.qml"))

                Layout.alignment: Qt.AlignHCenter
            }
        }
        
    }
}

}
