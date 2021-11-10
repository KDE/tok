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
        shortcut: settings.boldShortcut
        onTriggered: tClient.messagesStore.applyFormat("bold", formatMenu.field.textDocument, formatMenu.field, formatMenu.field.selectionStart, formatMenu.field.selectionEnd)
    }

    Labs.MenuItem {
        enabled: formatMenu.field !== null && formatMenu.field.selectedText !== ""
        text: i18nc("text editing menu action; applies format", "Italic")
        shortcut: settings.italicShortcut
        onTriggered: tClient.messagesStore.applyFormat("italic", formatMenu.field.textDocument, formatMenu.field, formatMenu.field.selectionStart, formatMenu.field.selectionEnd)
    }

    Labs.MenuItem {
        enabled: formatMenu.field !== null && formatMenu.field.selectedText !== ""
        text: i18nc("text editing menu action; applies format", "Underline")
        shortcut: settings.underlineShortcut
        onTriggered: tClient.messagesStore.applyFormat("underline", formatMenu.field.textDocument, formatMenu.field, formatMenu.field.selectionStart, formatMenu.field.selectionEnd)
    }

    Labs.MenuItem {
        enabled: formatMenu.field !== null && formatMenu.field.selectedText !== ""
        text: i18nc("text editing menu action; applies format", "Strikethrough")
        shortcut: settings.strikethroughShortcut
        onTriggered: tClient.messagesStore.applyFormat("strikethrough", formatMenu.field.textDocument, formatMenu.field, formatMenu.field.selectionStart, formatMenu.field.selectionEnd)
    }

    Labs.MenuItem {
        enabled: formatMenu.field !== null && formatMenu.field.selectedText !== ""
        text: i18nc("text editing menu action; applies format", "Monospace")
        shortcut: settings.monospaceShortcut
        onTriggered: tClient.messagesStore.applyFormat("monospace", formatMenu.field.textDocument, formatMenu.field, formatMenu.field.selectionStart, formatMenu.field.selectionEnd)
    }

    Labs.MenuSeparator {
    }

    Labs.MenuItem {
        enabled: formatMenu.field !== null && formatMenu.field.selectedText !== ""
        text: i18nc("text editing menu action; applies format", "Normal")
        shortcut: settings.normalShortcut
        onTriggered: tClient.messagesStore.applyFormat("normal", formatMenu.field.textDocument, formatMenu.field, formatMenu.field.selectionStart, formatMenu.field.selectionEnd)
    }
}