// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import Qt.labs.platform 1.1 as QQC2
import QtQuick 2.15
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.10
import org.kde.Tok 1.0 as Tok
import org.kde.kirigami 2.15 as Kirigami

QQC2.Menu {
    id: formatMenu

    required property TextEdit field

    title: i18nc("text menu editing action", "Format")
    enabled: formatMenu.field !== null

    QQC2.MenuItem {
        enabled: formatMenu.field !== null && formatMenu.field.selectedText !== ""
        text: i18nc("text editing menu action; applies format", "Bold")
        onTriggered: tClient.messagesStore.applyFormat("bold", formatMenu.field.textDocument, formatMenu.field, formatMenu.field.selectionStart, formatMenu.field.selectionEnd)
    }

    QQC2.MenuItem {
        enabled: formatMenu.field !== null && formatMenu.field.selectedText !== ""
        text: i18nc("text editing menu action; applies format", "Italic")
        onTriggered: tClient.messagesStore.applyFormat("italic", formatMenu.field.textDocument, formatMenu.field, formatMenu.field.selectionStart, formatMenu.field.selectionEnd)
    }

    QQC2.MenuItem {
        enabled: formatMenu.field !== null && formatMenu.field.selectedText !== ""
        text: i18nc("text editing menu action; applies format", "Underline")
        onTriggered: tClient.messagesStore.applyFormat("underline", formatMenu.field.textDocument, formatMenu.field, formatMenu.field.selectionStart, formatMenu.field.selectionEnd)
    }

    QQC2.MenuItem {
        enabled: formatMenu.field !== null && formatMenu.field.selectedText !== ""
        text: i18nc("text editing menu action; applies format", "Strikethrough")
        onTriggered: tClient.messagesStore.applyFormat("strikethrough", formatMenu.field.textDocument, formatMenu.field, formatMenu.field.selectionStart, formatMenu.field.selectionEnd)
    }

    QQC2.MenuItem {
        enabled: formatMenu.field !== null && formatMenu.field.selectedText !== ""
        text: i18nc("text editing menu action; applies format", "Monospace")
        onTriggered: tClient.messagesStore.applyFormat("monospace", formatMenu.field.textDocument, formatMenu.field, formatMenu.field.selectionStart, formatMenu.field.selectionEnd)
    }

    QQC2.MenuSeparator {
    }

    QQC2.MenuItem {
        enabled: formatMenu.field !== null && formatMenu.field.selectedText !== ""
        text: i18nc("text editing menu action; applies format", "Normal")
        onTriggered: tClient.messagesStore.applyFormat("normal", formatMenu.field.textDocument, formatMenu.field, formatMenu.field.selectionStart, formatMenu.field.selectionEnd)
    }
}