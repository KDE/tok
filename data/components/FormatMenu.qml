// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import Qt.labs.platform 1.1 as Labs
import QtQuick 2.15
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.10
import org.kde.Tok 1.0 as Tok
import org.kde.kirigami 2.15 as Kirigami

Labs.Menu {
    id: formatMenu

    required property TextEdit field

    title: i18nc("text menu editing action", "Format")
    enabled: formatMenu.field !== null

    Labs.MenuItem {
        enabled: formatMenu.field !== null && formatMenu.field.selectedText !== ""
        text: i18nc("text editing menu action; applies format", "Bold")
        shortcut: "Ctrl+B"
        onTriggered: tClient.messagesStore.applyFormat("bold", formatMenu.field.textDocument, formatMenu.field, formatMenu.field.selectionStart, formatMenu.field.selectionEnd)
    }

    Labs.MenuItem {
        enabled: formatMenu.field !== null && formatMenu.field.selectedText !== ""
        text: i18nc("text editing menu action; applies format", "Italic")
        shortcut: "Ctrl+I"
        onTriggered: tClient.messagesStore.applyFormat("italic", formatMenu.field.textDocument, formatMenu.field, formatMenu.field.selectionStart, formatMenu.field.selectionEnd)
    }

    Labs.MenuItem {
        enabled: formatMenu.field !== null && formatMenu.field.selectedText !== ""
        text: i18nc("text editing menu action; applies format", "Underline")
        shortcut: "Ctrl+U"
        onTriggered: tClient.messagesStore.applyFormat("underline", formatMenu.field.textDocument, formatMenu.field, formatMenu.field.selectionStart, formatMenu.field.selectionEnd)
    }

    Labs.MenuItem {
        enabled: formatMenu.field !== null && formatMenu.field.selectedText !== ""
        text: i18nc("text editing menu action; applies format", "Strikethrough")
        shortcut: "Ctrl+Shift+X"
        onTriggered: tClient.messagesStore.applyFormat("strikethrough", formatMenu.field.textDocument, formatMenu.field, formatMenu.field.selectionStart, formatMenu.field.selectionEnd)
    }

    Labs.MenuItem {
        enabled: formatMenu.field !== null && formatMenu.field.selectedText !== ""
        text: i18nc("text editing menu action; applies format", "Monospace")
        shortcut: "Ctrl+Shift+M"
        onTriggered: tClient.messagesStore.applyFormat("monospace", formatMenu.field.textDocument, formatMenu.field, formatMenu.field.selectionStart, formatMenu.field.selectionEnd)
    }

    Labs.MenuSeparator {
    }

    Labs.MenuItem {
        enabled: formatMenu.field !== null && formatMenu.field.selectedText !== ""
        text: i18nc("text editing menu action; applies format", "Normal")
        shortcut: "Ctrl+Shift+N"
        onTriggered: tClient.messagesStore.applyFormat("normal", formatMenu.field.textDocument, formatMenu.field, formatMenu.field.selectionStart, formatMenu.field.selectionEnd)
    }
}