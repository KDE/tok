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
            text: i18nc("title", "Proxy Settings")
            level: 4
        }
        Item { Layout.fillWidth: true }
    }

    ColumnLayout {
        spacing: 0

        Kirigami.Icon {
            source: "preferences-system-network-proxy"

            Layout.preferredHeight: Kirigami.Units.iconSizes.huge
            Layout.preferredWidth: Kirigami.Units.iconSizes.huge
            Layout.margins: Kirigami.Units.gridUnit

            Layout.alignment: Qt.AlignHCenter
        }

        Kirigami.Heading {
            level: 4
            text: i18nc("header", "Current Proxies")

            Layout.bottomMargin: Kirigami.Units.gridUnit
            Layout.leftMargin: Kirigami.Units.largeSpacing
        }
        Kirigami.PlaceholderMessage {
            visible: repeater.count === 0
            text: i18nc("mobile message, keep it as thin as possible", "You have no proxies added.")
        }
        Repeater {
            id: repeater
            model: tClient.proxyModel
            delegate: Kirigami.BasicListItem {
                    text: `${model.server}:${model.port}`
                    property string protocolText: {
                        return {
                            "http": i18n("HTTP"),
                            "mtproto": i18n("MTProto"),
                            "socks5": i18n("SOCKS5"),
                        }[model.kind]
                    }
                    subtitle: model.enabled ? i18n("Active %1 Proxy", protocolText) : i18n("%1 Proxy", protocolText)

                    trailing: Row {
                        QQC2.Button {
                            anchors.verticalCenter: parent.verticalCenter
                            text: model.enabled ? i18nc("button", "Disable") : i18nc("button", "Enable")
                            onClicked: model.enabled = !model.enabled
                        }
                        QQC2.Button {
                            anchors.verticalCenter: parent.verticalCenter
                            icon.name: "delete"
                            onClicked: model.deleted = true
                        }
                    }
                }
        }

        Item { Layout.preferredHeight: Kirigami.Units.gridUnit * 2 }
        Kirigami.BasicListItem {
            text: i18n("New Proxy")
            onClicked: rootRow.layers.push(Qt.resolvedUrl("NewProxy.qml"))

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