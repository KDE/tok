// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import Qt.labs.platform 1.1 as QQC2
import QtQuick 2.15
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.10
import org.kde.Tok 1.0 as Tok
import org.kde.kirigami 2.15 as Kirigami

QQC2.Menu {
    id: editMenu

    required property Item field
    required property bool showFormat

    QQC2.MenuItem {
        enabled: editMenu.field !== null && editMenu.field.canUndo
        text: i18nc("text editing menu action", "Undo")
        onTriggered: {
            editMenu.field.undo()
            editMenu.close()
        }
    }

    QQC2.MenuItem {
        enabled: editMenu.field !== null && editMenu.field.canRedo
        text: i18nc("text editing menu action", "Redo")
        onTriggered: {
            editMenu.field.undo()
            editMenu.close()
        }
    }

    QQC2.MenuSeparator {
    }

    QQC2.MenuItem {
        enabled: editMenu.field !== null && editMenu.field.selectedText
        text: i18nc("text editing menu action", "Cut")
        onTriggered: {
            editMenu.field.cut()
            editMenu.close()
        }
    }

    QQC2.MenuItem {
        enabled: editMenu.field !== null && editMenu.field.selectedText
        text: i18nc("text editing menu action", "Copy")
        onTriggered: {
            editMenu.field.copy()
            editMenu.close()
        }
    }

    QQC2.MenuItem {
        enabled: editMenu.field !== null && editMenu.field.canPaste
        text: i18nc("text editing menu action", "Paste")
        onTriggered: {
            editMenu.field.paste()
            editMenu.close()
        }
    }

    QQC2.MenuItem {
        enabled: editMenu.field !== null && editMenu.field.selectedText !== ""
        text: i18nc("text editing menu action", "Delete")
        onTriggered: {
            editMenu.field.remove(editMenu.field.selectionStart, editMenu.field.selectionEnd)
            editMenu.close()
        }
    }

    QQC2.MenuSeparator {
        visible: editMenu.showFormat
    }

    FormatMenuInWindow {
        visible: editMenu.showFormat
        field: editMenu.field instanceof TextEdit ? editMenu.field : null
    }

    QQC2.MenuSeparator {
    }

    QQC2.MenuItem {
        enabled: editMenu.field !== null
        text: i18nc("text editing menu action", "Select All")
        onTriggered: {
            editMenu.field.selectAll()
            editMenu.close()
        }
    }

    QQC2.MenuSeparator {
        visible: !editMenu.field.readOnly
    }

    QQC2.Menu {
        id: correctionsMenu

        property var suggestions: []

        Connections {
            target: editMenu.field
            function onCursorPositionChanged() {
                correctionsMenu.suggestions = theOneTrueSpellCheckHighlighter.suggestions(editMenu.field.cursorPosition)
            }
        }

        title: i18nc("text editing submenu", "Spellchecking")
        visible: !editMenu.field.readOnly && theOneTrueSpellCheckHighlighter.active

        Instantiator {
            active: !editMenu.field.readOnly && theOneTrueSpellCheckHighlighter.active && theOneTrueSpellCheckHighlighter.wordIsMisspelled
            model: correctionsMenu.suggestions
            delegate: QQC2.MenuItem {
                text: modelData
                onTriggered: theOneTrueSpellCheckHighlighter.replaceWord(modelData)
            }
            onObjectAdded: correctionsMenu.insertItem(index, object)
            onObjectRemoved: correctionsMenu.removeItem(object)
        }

        QQC2.MenuItem {
            visible: theOneTrueSpellCheckHighlighter.wordIsMisspelled && correctionsMenu.suggestions.length === 0
            text: i18n("No suggestions for \"%1\"", theOneTrueSpellCheckHighlighter.wordUnderMouse)
            enabled: false
        }

        QQC2.MenuSeparator {
        }

        QQC2.MenuItem {
            visible: theOneTrueSpellCheckHighlighter.wordIsMisspelled
            text: i18n("Add \"%1\" to dictionary", theOneTrueSpellCheckHighlighter.wordUnderMouse)
            onTriggered: theOneTrueSpellCheckHighlighter.addWordToDictionary(theOneTrueSpellCheckHighlighter.wordUnderMouse)
        }

        QQC2.MenuItem {
            visible: theOneTrueSpellCheckHighlighter.wordIsMisspelled
            text: i18n("Ignore \"%1\"", theOneTrueSpellCheckHighlighter.wordUnderMouse)
            onTriggered: theOneTrueSpellCheckHighlighter.ignoreWord(theOneTrueSpellCheckHighlighter.wordUnderMouse)
        }
    }
}
