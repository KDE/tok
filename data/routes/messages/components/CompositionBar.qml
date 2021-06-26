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
            active: messagesViewRoot.replyToID != ""
            visible: messagesViewRoot.replyToID != ""

            sourceComponent: QQC2.Control {
                padding: 6
                contentItem: RowLayout {
                    spacing: 6

                    GlobalComponents.PlaintextMessage {
                        id: repliedToData

                        messagesModel: tClient.messagesStore
                        userModel: tClient.userDataModel
                        chatID: messagesViewRoot.chatID
                        messageID: messagesViewRoot.replyToID
                    }
                    Kirigami.Icon {
                        source: "dialog-messages"
                    }
                    ColumnLayout {
                        spacing: 1
                        QQC2.Label {
                            text: repliedToData.authorName
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
                        onClicked: messagesViewRoot.replyToID = ""
                    }

                    Layout.fillWidth: true
                }
                Layout.fillWidth: true
            }
        }

        RowLayout {
            id: composeRow

            function send() {
                if (messagesViewRoot.uploadPath != "") {
                    if (messagesViewRoot.isPhoto) {
                        lView.model.sendPhoto(txtField.textDocument, messagesViewRoot.uploadPath, messagesViewRoot.replyToID)
                    } else {
                        lView.model.sendFile(txtField.textDocument, messagesViewRoot.uploadPath, messagesViewRoot.replyToID)
                    }

                    messagesViewRoot.uploadPath = ""
                    txtField.text = ""
                    messagesViewRoot.replyToID = ""
                    return
                }
                lView.model.send(txtField.textDocument, messagesViewRoot.replyToID)
                txtField.text = ""
                messagesViewRoot.replyToID = ""
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
            }
            QQC2.ToolButton {
                Accessible.name: i18n("Upload file")
                icon.name: "mail-attachment"
                onClicked: {
                    messagesViewRoot.isPhoto = false
                    Tok.Utils.pickFile(i18nc("Dialog title", "Upload file"), "file").then((url) => {
                        messagesViewRoot.uploadPath = url
                        composeRow.send()
                    })
                }
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
                        messagesViewRoot.uploadPath = clipboard.urls[0]
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