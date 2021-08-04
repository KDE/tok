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

Kirigami.FormLayout {
    id: formLayout

    width: 600

    property alias data_server: server.text
    property alias data_port: port.text
    property alias data_enabled: activeBox.checked
    property alias data_kind: kindBox.currentValue
    property alias data_mtproto_Secret: mtprotoSecret.text
    property alias data_http_Username: httpUsername.text
    property alias data_http_Password: httpPassword.text
    property alias data_http_HTTPOnly: httpHttpOnly.checked
    property alias data_socks55_Username: socks5Username.text
    property alias data_socks55_Password: socks5Password.text

    function reset() {
        data_server = ""
        data_port = ""
        data_enabled = false
        data_mtproto_Secret = ""
        data_http_Username = ""
        data_http_Password = ""
        data_http_HTTPOnly = false
        data_socks55_Username = ""
        data_socks55_Password = ""
    }

    readonly property var typeData: {
        return {
            "kind": formLayout.data_kind,
            "mtproto_Secret": formLayout.data_mtproto_Secret,
            "http_Username": formLayout.data_http_Username,
            "http_Password": formLayout.data_http_Password,
            "http_HTTPOnly": formLayout.data_http_HTTPOnly,
            "socks55_Username": formLayout.data_socks55_Username,
            "socks55_Password": formLayout.data_socks55_Password,
        }
    }

    Item {
        Kirigami.FormData.label: i18nc("form subheader", "General Configuration")
        Kirigami.FormData.isSection: true
    }
    QQC2.TextField {
        id: server
        Layout.maximumWidth: 200
        Kirigami.FormData.label: i18nc("form label", "Server:")
    }
    QQC2.TextField {
        id: port
        Layout.maximumWidth: 200
        Kirigami.FormData.label: i18nc("form label", "Port:")
        validator: IntValidator {
        }
    }
    QQC2.CheckBox {
        id: activeBox
        text: i18nc("checkbox label", "Make this proxy the active one when added")
    }
    Item {
        Kirigami.FormData.isSection: true
    }
    QQC2.ComboBox {
        id: kindBox
        model: ["http", "mtproto", "socks5"]
        Kirigami.FormData.label: i18nc("form label", "Proxy Type:")
    }
    Item {
        Kirigami.FormData.label: {
            const obj = {
                "http": i18nc("form subheader", "HTTP Configuration"),
                "mtproto": i18nc("form subheader", "MTProto Configuration"),
                "socks5": i18nc("form subheader", "SOCKS5 Configuration"),
            }
            return obj[kindBox.currentValue]
        }
        Kirigami.FormData.isSection: true
    }
    QQC2.TextField {
        id: mtprotoSecret
        visible: kindBox.currentValue === "mtproto"
        Layout.maximumWidth: 200
        Kirigami.FormData.label: i18nc("form control label", "MTProto Secret")
    }
    QQC2.TextField {
        id: httpUsername
        visible: kindBox.currentValue === "http"
        Layout.maximumWidth: 200
        Kirigami.FormData.label: i18nc("form control label", "Username:")
    }
    Kirigami.PasswordField {
        id: httpPassword
        visible: kindBox.currentValue === "http"
        Layout.maximumWidth: 200
        Kirigami.FormData.label: i18nc("form control label", "Password:")
    }
    QQC2.CheckBox {
        id: httpHttpOnly
        visible: kindBox.currentValue === "http"
        text: i18nc("form control label", "This proxy only supports plain HTTP requests")
    }
    QQC2.Label {
        visible: kindBox.currentValue === "http"
        text: i18nc("additional explanatory text below checkbox", "Ask your proxy provider if you're unsure about this option")
        elide: Text.ElideRight
        font: Kirigami.Theme.smallFont
        Layout.fillWidth: true
        Layout.minimumWidth: Kirigami.Units.gridUnit * 15
    }

    QQC2.TextField {
        id: socks5Username
        visible: kindBox.currentValue === "socks5"
        Layout.maximumWidth: 200
        Kirigami.FormData.label: i18nc("form control label", "Username:")
    }
    QQC2.TextField {
        id: socks5Password
        Layout.maximumWidth: 200
        visible: kindBox.currentValue === "socks5"
        Kirigami.FormData.label: i18nc("form control label", "Password:")
    }
}