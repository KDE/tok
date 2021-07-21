// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.10
import org.kde.Tok 1.0 as Tok
import org.kde.kirigami 2.15 as Kirigami
import QtQuick.Dialogs 1.3
import "qrc:/components" as GlobalComponents

QQC2.TextArea {
    id: txtField

    activeFocusOnTab: true
    persistentSelection: true
    wrapMode: Text.Wrap

    color: Kirigami.Theme.textColor
    selectedTextColor: Kirigami.Theme.highlightedTextColor
    selectionColor: Kirigami.Theme.highlightColor

    Layout.preferredWidth: 300

    GlobalComponents.EditMenu {
        id: editMenu

        field: txtField
        showFormat: true
    }

    placeholderText: i18n("Add a caption…")

    MouseArea {
        anchors.fill: parent

        enabled: !txtField.readOnly
        acceptedButtons: Qt.RightButton

        onPressed: (ev) => {
            ev.accepted = true
            editMenu.open()
        }
        onReleased: (ev) => {
            ev.accepted = true
        }
    }

    Shortcut {
        sequence: "Ctrl+B"
        context: Qt.ApplicationShortcut
        onActivated: {
            tClient.messagesStore.applyFormat("bold", txtField.textDocument, txtField, txtField.selectionStart, txtField.selectionEnd)
        }
    }
    Shortcut {
        sequence: "Ctrl+I"
        context: Qt.ApplicationShortcut
        onActivated: {
            tClient.messagesStore.applyFormat("italic", txtField.textDocument, txtField, txtField.selectionStart, txtField.selectionEnd)
        }
    }
    Shortcut {
        sequence: "Ctrl+U"
        context: Qt.ApplicationShortcut
        onActivated: {
            tClient.messagesStore.applyFormat("underline", txtField.textDocument, txtField, txtField.selectionStart, txtField.selectionEnd)
        }
    }
    Shortcut {
        sequence: "Ctrl+Shift+X"
        context: Qt.ApplicationShortcut
        onActivated: {
            tClient.messagesStore.applyFormat("strikethrough", txtField.textDocument, txtField, txtField.selectionStart, txtField.selectionEnd)
        }
    }
    Shortcut {
        sequence: "Ctrl+Shift+M"
        context: Qt.ApplicationShortcut
        onActivated: {
            tClient.messagesStore.applyFormat("monospace", txtField.textDocument, txtField, txtField.selectionStart, txtField.selectionEnd)
        }
    }
    Shortcut {
        sequence: "Ctrl+Shift+N"
        context: Qt.ApplicationShortcut
        onActivated: {
            tClient.messagesStore.applyFormat("normal", txtField.textDocument, txtField, txtField.selectionStart, txtField.selectionEnd)
        }
    }

    Keys.onMenuPressed: editMenu.open()
    Keys.onReturnPressed: (event) => {
        if (!(event.modifiers & Qt.ShiftModifier)) {
            // composeRow.send()
            event.accepted = true
        } else {
            event.accepted = false
        }
    }
    Layout.fillWidth: true
}