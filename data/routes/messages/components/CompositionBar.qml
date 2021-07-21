// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.15 as Kirigami
import org.kde.Tok 1.0 as Tok

import "qrc:/components" as GlobalComponents

QQC2.ToolBar {
    id: composeBar

    property alias text: txtField.text

    Loader {
        id: autoCompleteThing
        active: false

        parent: composeBar

        property string filter: ""
        function up(event) {
            if (this.item != null) {
                return this.item.up(event)
            }
            return false
        }
        function down(event) {
            if (this.item != null) {
                return this.item.down(event)
            }
            return false
        }
        function tab(event) {
            if (this.item != null) {
                return this.item.tab(event)
            }
            return false
        }

        sourceComponent: MentionBar {
            clip: true

            parent: composeBar
            height: 200
            filter: autoCompleteThing.filter

            anchors {
                bottom: parent.top
                left: parent.left
                right: parent.right
            }
        }
    }

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, contentItem.implicitHeight + topPadding + bottomPadding)

    contentItem: ColumnLayout {
        GlobalComponents.LoaderPlus {
            active: messagesViewRoot.interactionID != ""
            visible: messagesViewRoot.interactionID != ""

            sourceComponent: QQC2.Control {
                padding: 6
                contentItem: RowLayout {
                    spacing: 6

                    GlobalComponents.PlaintextMessage {
                        id: repliedToData

                        messagesModel: tClient.messagesStore
                        userModel: tClient.userDataModel
                        chatID: messagesViewRoot.chatID
                        messageID: messagesViewRoot.interactionID
                    }
                    Kirigami.Icon {
                        source: messagesViewRoot.interactionKind === "reply" ? "dialog-messages" : "edit-entry"
                    }
                    ColumnLayout {
                        spacing: 1
                        QQC2.Label {
                            text: messagesViewRoot.interactionKind == "edit" ? i18n("Edit Message") : repliedToData.authorName
                            elide: Text.ElideRight
                            color: Kirigami.NameUtils.colorsFromString(repliedToData.authorName)
                            Layout.fillWidth: true
                        }
                        QQC2.Label {
                            text: repliedToData.onelinePlaintext
                            elide: Text.ElideRight
                            textFormat: TextEdit.MarkdownText
                            Layout.fillWidth: true
                        }

                        clip: true
                        Layout.fillWidth: true
                    }
                    QQC2.ToolButton {
                        icon.name: "dialog-cancel"
                        onClicked: {
                            messagesViewRoot.interactionID = ""
                            if (messagesViewRoot.interactionKind == "edit") {
                                txtField.text = ""
                            }
                            messagesViewRoot.interactionKind = ""
                        }
                    }

                    Layout.fillWidth: true
                }
                Layout.fillWidth: true
            }
        }

        RowLayout {
            id: composeRow

            function send() {
                if (messagesViewRoot.interactionKind === "edit") {
                    lView.model.edit(txtField.textDocument, messagesViewRoot.interactionID)

                    messagesViewRoot.uploadPath = ""
                    txtField.text = ""
                    messagesViewRoot.interactionID = ""
                    messagesViewRoot.interactionKind = ""
                    return
                }
                if (messagesViewRoot.uploadPath != "") {
                    if (messagesViewRoot.isPhoto) {
                        lView.model.sendPhoto(txtField.textDocument, messagesViewRoot.uploadPath, messagesViewRoot.interactionID)
                    } else {
                        lView.model.sendFile(txtField.textDocument, messagesViewRoot.uploadPath, messagesViewRoot.interactionID)
                    }

                    messagesViewRoot.uploadPath = ""
                    txtField.text = ""
                    messagesViewRoot.interactionID = ""
                    messagesViewRoot.interactionKind = ""
                    return
                }
                lView.model.send(txtField.textDocument, messagesViewRoot.interactionID)
                txtField.text = ""
                messagesViewRoot.interactionID = ""
                messagesViewRoot.interactionKind = ""
            }

            Layout.fillWidth: true

            QQC2.ToolButton {
                Accessible.name: i18n("Upload photo")
                icon.name: "photo"
                onClicked: {
                    messagesViewRoot.isPhoto = true
                    Tok.Utils.pickFile(i18nc("Dialog title", "Upload photo"), "photo").then((url) => {
                        messagesViewRoot.uploadPath = url
                        composeRow.send()
                    })
                }
                visible: Kirigami.Settings.isMobile
            }
            QQC2.ToolButton {
                Accessible.name: i18n("Upload media")
                icon.name: "mail-attachment"
                onClicked: {
                    messagesViewRoot.isPhoto = false
                    Tok.Utils.pickFile(i18nc("Dialog title", "Upload file"), "file").then((url) => {
                        messagesViewRoot.uploadPath = url
                        composeRow.send()
                    })
                }
                visible: Kirigami.Settings.isMobile
            }

            QQC2.ToolButton {
                Accessible.name: i18n("Attach file")
                icon.name: "mail-attachment"
                onClicked: {
                    desktopPicker.chatID = messagesViewRoot.chatID
                    desktopPicker.pick()
                }
                visible: !Kirigami.Settings.isMobile
            }

            TextEdit {
                id: txtField

                activeFocusOnTab: true
                persistentSelection: true
                enabled: chatData.data.mCanSendMessages
                selectByMouse: !Kirigami.Settings.isMobile
                wrapMode: Text.Wrap

                color: Kirigami.Theme.textColor
                selectedTextColor: Kirigami.Theme.highlightedTextColor
                selectionColor: Kirigami.Theme.highlightColor

                GlobalComponents.EditMenu {
                    id: editMenu

                    field: txtField
                    showFormat: true
                }

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

                QQC2.Label {
                    visible: !txtField.text

                    text: enabled ? i18n("Write your message…") : i18n("You cannot send messages.")

                    opacity: 0.5
                }

                Tok.Clipboard.paste: function(clipboard) {
                    if (clipboard.hasUrls) {
                        desktopPicker.chatID = messagesViewRoot.chatID
                        desktopPicker.source = clipboard.urls[0]
                        desktopPicker.open()

                        return true
                    } else if (clipboard.hasImage) {
                        messagesViewRoot.isPhoto = true
                        messagesViewRoot.uploadPath = clipboard.imageUrl
                        composeRow.send()

                        return true
                    }
                }

                onCursorPositionChanged: {
                    doAutocomplete()
                }
                onTextChanged: {
                    doAutocomplete()
                }

                function doAutocomplete() {
                    autoCompleteThing.active = Tok.Utils.wordAt(cursorPosition, text)[0] == '@'
                    autoCompleteThing.filter = Tok.Utils.wordAt(cursorPosition, text).slice(1)
                }

                Keys.onMenuPressed: editMenu.open()
                Keys.onReturnPressed: (event) => {
                    if (!(event.modifiers & Qt.ShiftModifier)) {
                        composeRow.send()
                        event.accepted = true
                    } else {
                        event.accepted = false
                    }
                }
                Keys.onUpPressed: (event) => autoCompleteThing.up(event)
                Keys.onDownPressed: (event) => autoCompleteThing.down(event)
                Keys.onTabPressed: (event) => {
                    if (autoCompleteThing.tab(event)) {
                        return
                    }
                    nextItemInFocusChain().forceActiveFocus(Qt.TabFocusReason)
                }
                Layout.fillWidth: true
            }
            QQC2.Button {
                Accessible.name: i18n("Send message")
                icon.name: "document-send"
                onClicked: composeRow.send()
            }
        }
    }
}