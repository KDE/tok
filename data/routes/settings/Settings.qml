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

    title: i18nc("window title", "Preferences")

    width: Kirigami.Units.gridUnit * 30
    height: Kirigami.Units.gridUnit * 20

    header: Kirigami.Separator {
        anchors {
            left: parent.left
            right: parent.right
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        QQC2.ScrollView {
            Layout.fillHeight: true

            Kirigami.Theme.inherit: false
            Kirigami.Theme.colorSet: Kirigami.Theme.View

            background: Rectangle {
                color: Kirigami.Theme.backgroundColor
            }

            ColumnLayout {
                spacing: 0

                SidebarItem {
                    pageName: i18n("Account")
                    accessibleDescription: i18nc("sidebar item accessible text", "Open account settings")
                    icon.name: "preferences-system-users"
                    page: Qt.resolvedUrl("AccountSettings.qml")
                }
                SidebarItem {
                    pageName: i18n("Appearance")
                    accessibleDescription: i18nc("sidebar item accessible text", "Open appearance settings")
                    icon.name: "preferences-desktop-color"
                    page: Qt.resolvedUrl("AppearanceSettings.qml")
                }
                SidebarItem {
                    pageName: i18n("Behaviour")
                    accessibleDescription: i18nc("sidebar item accessible text", "Open app behaviour settings")
                    icon.name: "preferences"
                    page: Qt.resolvedUrl("BehaviourSettings.qml")
                }
                SidebarItem {
                    pageName: i18n("Proxy")
                    accessibleDescription: i18nc("sidebar item accessible text", "Open proxy settings")
                    icon.name: "preferences-system-network-proxy"
                    page: Qt.resolvedUrl("ProxySettings.qml")
                }
            }
        }
        Kirigami.Separator {
            Layout.fillHeight: true
        }
        Loader {
            id: pageLoader
            source: Qt.resolvedUrl("AppearanceSettings.qml")
            asynchronous: true
            onStatusChanged: if (this.status == Loader.Error) Qt.quit(1)

            Layout.margins: item && item.noMargin ? 0 : Kirigami.Units.largeSpacing
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}