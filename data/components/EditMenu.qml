// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import Qt.labs.platform 1.1 as Labs
import QtQuick 2.15
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.10
import org.kde.Tok 1.0 as Tok
import org.kde.kirigami 2.15 as Kirigami

Labs.Menu {
    id: editMenu

    required property TextEdit field

    Labs.MenuItem {
        enabled: editMenu.field.canUndo
        text: i18nc("text editing menu action", "Undo")
        shortcut: StandardKey.Undo
        onTriggered: {
            editMenu.field.undo()
            editMenu.close()
        }
    }

    Labs.MenuItem {
        enabled: editMenu.field.canRedo
        text: i18nc("text editing menu action", "Redo")
        shortcut: StandardKey.Redo
        onTriggered: {
            editMenu.field.undo()
            editMenu.close()
        }
    }

    Labs.MenuSeparator {
    }

    Labs.MenuItem {
        enabled: editMenu.field.selectedText
        text: i18nc("text editing menu action", "Cut")
        shortcut: StandardKey.Cut
        onTriggered: {
            editMenu.field.cut()
            editMenu.close()
        }
    }

    Labs.MenuItem {
        enabled: editMenu.field.selectedText
        text: i18nc("text editing menu action", "Copy")
        shortcut: StandardKey.Copy
        onTriggered: {
            editMenu.field.copy()
            editMenu.close()
        }
    }

    Labs.MenuItem {
        enabled: editMenu.field.canPaste
        text: i18nc("text editing menu action", "Paste")
        shortcut: StandardKey.Paste
        onTriggered: {
            editMenu.field.paste()
            editMenu.close()
        }
    }

    Labs.MenuItem {
        enabled: editMenu.field.selectedText !== ""
        text: i18nc("text editing menu action", "Delete")
        shortcut: ""
        onTriggered: {
            editMenu.field.remove(editMenu.field.selectionStart, editMenu.field.selectionEnd)
            editMenu.close()
        }
    }

    Labs.MenuSeparator {
    }

    Labs.Menu {
        title: i18nc("text menu editing action", "Format")

        Labs.MenuItem {
            enabled: editMenu.field.selectedText !== ""
            text: i18nc("text editing menu action; applies format", "Bold")
            shortcut: "Ctrl+B"
            onTriggered: tClient.messagesStore.applyFormat("bold", txtField.textDocument, txtField, txtField.selectionStart, txtField.selectionEnd)
        }

        Labs.MenuItem {
            enabled: editMenu.field.selectedText !== ""
            text: i18nc("text editing menu action; applies format", "Italic")
            shortcut: "Ctrl+I"
            onTriggered: tClient.messagesStore.applyFormat("italic", txtField.textDocument, txtField, txtField.selectionStart, txtField.selectionEnd)
        }

        Labs.MenuItem {
            enabled: editMenu.field.selectedText !== ""
            text: i18nc("text editing menu action; applies format", "Underline")
            shortcut: "Ctrl+U"
            onTriggered: tClient.messagesStore.applyFormat("underline", txtField.textDocument, txtField, txtField.selectionStart, txtField.selectionEnd)
        }

        Labs.MenuItem {
            enabled: editMenu.field.selectedText !== ""
            text: i18nc("text editing menu action; applies format", "Strikethrough")
            shortcut: "Ctrl+Shift+X"
            onTriggered: tClient.messagesStore.applyFormat("strikethrough", txtField.textDocument, txtField, txtField.selectionStart, txtField.selectionEnd)
        }

        Labs.MenuItem {
            enabled: editMenu.field.selectedText !== ""
            text: i18nc("text editing menu action; applies format", "Monospace")
            shortcut: "Ctrl+Shift+M"
            onTriggered: tClient.messagesStore.applyFormat("monospace", txtField.textDocument, txtField, txtField.selectionStart, txtField.selectionEnd)
        }

        Labs.MenuSeparator {
        }

        Labs.MenuItem {
            enabled: editMenu.field.selectedText !== ""
            text: i18nc("text editing menu action; applies format", "Normal")
            shortcut: "Ctrl+Shift+N"
            onTriggered: tClient.messagesStore.applyFormat("normal", txtField.textDocument, txtField, txtField.selectionStart, txtField.selectionEnd)
        }

    }

    Labs.MenuSeparator {
    }

    Labs.MenuItem {
        text: i18nc("text editing menu action", "Select All")
        shortcut: StandardKey.SelectAll
        onTriggered: {
            editMenu.field.selectAll()
            editMenu.close()
        }
    }

}
