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

    required property Item field
    required property bool showFormat

    Labs.MenuItem {
        enabled: editMenu.field !== null && editMenu.field.canUndo
        text: i18nc("text editing menu action", "Undo")
        shortcut: StandardKey.Undo
        onTriggered: {
            editMenu.field.undo()
            editMenu.close()
        }
    }

    Labs.MenuItem {
        enabled: editMenu.field !== null && editMenu.field.canRedo
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
        enabled: editMenu.field !== null && editMenu.field.selectedText
        text: i18nc("text editing menu action", "Cut")
        shortcut: StandardKey.Cut
        onTriggered: {
            editMenu.field.cut()
            editMenu.close()
        }
    }

    Labs.MenuItem {
        enabled: editMenu.field !== null && editMenu.field.selectedText
        text: i18nc("text editing menu action", "Copy")
        shortcut: StandardKey.Copy
        onTriggered: {
            editMenu.field.copy()
            editMenu.close()
        }
    }

    Labs.MenuItem {
        enabled: editMenu.field !== null && editMenu.field.canPaste
        text: i18nc("text editing menu action", "Paste")
        shortcut: StandardKey.Paste
        onTriggered: {
            editMenu.field.paste()
            editMenu.close()
        }
    }

    Labs.MenuItem {
        enabled: editMenu.field !== null && editMenu.field.selectedText !== ""
        text: i18nc("text editing menu action", "Delete")
        shortcut: ""
        onTriggered: {
            editMenu.field.remove(editMenu.field.selectionStart, editMenu.field.selectionEnd)
            editMenu.close()
        }
    }

    Labs.MenuSeparator {
        visible: editMenu.showFormat
    }

    FormatMenu {
        visible: editMenu.showFormat
        field: editMenu.field instanceof TextEdit ? editMenu.field : null
    }

    Labs.MenuSeparator {
    }

    Labs.MenuItem {
        enabled: editMenu.field !== null
        text: i18nc("text editing menu action", "Select All")
        shortcut: StandardKey.SelectAll
        onTriggered: {
            editMenu.field.selectAll()
            editMenu.close()
        }
    }

    Labs.MenuSeparator {
        visible: !editMenu.field.readOnly
    }

    Labs.Menu {
        id: correctionsMenu

        title: i18nc("text editing submenu", "Spellchecking")
        visible: !editMenu.field.readOnly && theOneTrueSpellCheckHighlighter.active

        Instantiator {
            active: !editMenu.field.readOnly && theOneTrueSpellCheckHighlighter.active && theOneTrueSpellCheckHighlighter.wordIsMisspelled
            model: theOneTrueSpellCheckHighlighter.suggestions_
            delegate: Labs.MenuItem {
                text: modelData
                onTriggered: theOneTrueSpellCheckHighlighter.replaceWord(modelData)
            }
            onObjectAdded: correctionsMenu.insertItem(index, object)
            onObjectRemoved: correctionsMenu.removeItem(object)
        }

        Labs.MenuItem {
            visible: theOneTrueSpellCheckHighlighter.wordIsMisspelled && theOneTrueSpellCheckHighlighter.suggestions_.length === 0
            text: i18n("No suggestions for \"%1\"", theOneTrueSpellCheckHighlighter.wordUnderMouse)
            enabled: false
        }

        Labs.MenuSeparator {
        }

        Labs.MenuItem {
            visible: theOneTrueSpellCheckHighlighter.wordIsMisspelled
            text: i18n("Add \"%1\" to dictionary", theOneTrueSpellCheckHighlighter.wordUnderMouse)
            onTriggered: theOneTrueSpellCheckHighlighter.addWordToDictionary(theOneTrueSpellCheckHighlighter.wordUnderMouse)
        }

        Labs.MenuItem {
            visible: theOneTrueSpellCheckHighlighter.wordIsMisspelled
            text: i18n("Ignore \"%1\"", theOneTrueSpellCheckHighlighter.wordUnderMouse)
            onTriggered: theOneTrueSpellCheckHighlighter.ignoreWord(theOneTrueSpellCheckHighlighter.wordUnderMouse)
        }
    }
}
