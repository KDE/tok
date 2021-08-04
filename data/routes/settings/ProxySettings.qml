// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.3 as QQC2
import QtQuick.Window 2.2
import org.kde.Tok 1.0 as Tok
import Qt.labs.qmlmodels 1.0

import org.kde.kirigami 2.12 as Kirigami

Item {
    readonly property bool noMargin: true

    QQC2.ScrollView {
        ListView {
            model: tClient.proxyModel
            delegate: chooser

            Kirigami.PlaceholderMessage {
                visible: parent.count === 0
                text: i18nc("placeholder message", "You have no proxies added. Use the Add Proxy button at the bottom of the window to add one.")

                anchors.fill: parent
                anchors.margins: Kirigami.Units.gridUnit
            }
        }

        anchors.fill: parent
        anchors.bottomMargin: tBar.height
        QQC2.ScrollBar.horizontal.policy: QQC2.ScrollBar.AlwaysOff
    }

    component ProxyItemDelegate : Kirigami.BasicListItem {
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

    DelegateChooser {
        id: chooser
        role: "kind"
        DelegateChoice { roleValue: "http"; ProxyItemDelegate { } }
        DelegateChoice { roleValue: "mtproto"; ProxyItemDelegate { } }
        DelegateChoice { roleValue: "socks"; ProxyItemDelegate { } }
    }

    QQC2.ToolBar {
        id: tBar

        position: QQC2.ToolBar.Footer
        width: parent.width
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        contentItem: RowLayout {
            QQC2.Button {
                onClicked: addProxyWindow.doOpen()
                text: i18nc("adds a proxy", "Add Proxy")
            }
        }
    }

    Kirigami.ApplicationWindow {
        id: addProxyWindow
        visible: false

        title: i18nc("window title", "Add Proxy")

        function doOpen() {
            this.visible = true
            formLayout.reset()
        }
        function doClose(ok) {
            this.visible = false
            if (!ok) {
                return
            }
            tClient.proxyModel.insert(formLayout.data_server, formLayout.data_port, formLayout.data_enabled, formLayout.typeData)
        }

        width: 600
        minimumWidth: 600
        maximumWidth: 600

        height: formLayout.implicitHeight + Kirigami.Units.gridUnit * 4
        minimumHeight: formLayout.implicitHeight + Kirigami.Units.gridUnit * 4
        maximumHeight: formLayout.implicitHeight + Kirigami.Units.gridUnit * 4

        ProxyForm {
            id: formLayout
        }

        footer: QQC2.ToolBar {
            position: QQC2.ToolBar.Footer
            contentItem: RowLayout {
                Item { Layout.fillWidth: true }
                QQC2.Button {
                    text: i18nc("button", "Cancel")
                    onClicked: addProxyWindow.doClose(false)
                }
                QQC2.Button {
                    text: i18nc("button", "Add Proxy")
                    onClicked: addProxyWindow.doClose(true)
                }
            }
        }

        header: Kirigami.Separator {
            anchors {
                left: parent.left
                right: parent.right
            }
        }
    }

    Layout.fillHeight: true
    Layout.fillWidth: true
}
