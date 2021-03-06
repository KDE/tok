// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtGraphicalEffects 1.15
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

import "../shared" as Shared
import "../link_activation.mjs" as LinkActivation

QQC2.Control {
    id: textMessageRoot

    topPadding: Kirigami.Units.largeSpacing
    bottomPadding: Kirigami.Units.largeSpacing
    leftPadding: Kirigami.Units.largeSpacing+2+tailSize
    rightPadding: Kirigami.Units.largeSpacing+2

    readonly property int tailSize: Kirigami.Units.largeSpacing

    Connections {
        target: del
        function onEdit() {
            composeBar.text = textData.data.content
        }
    }

    Accessible.name: `${userData.data.name}: ${textData.data.content}. ${messageData.data.timestamp}`

    Tok.RelationalListener {
        id: textData

        model: tClient.messagesStore
        key: [del.mChatID, del.mID]
        shape: QtObject {
            required property string content
        }
    }

    background: MessageBackground {
        id: _background
        tailSize: textMessageRoot.tailSize
    }
    contentItem: ColumnLayout {
        QQC2.Label {
            text: userData.data.name
            color: Kirigami.NameUtils.colorsFromString(text)

            visible: del.separateFromPrevious && !(del.isOwnMessage && Kirigami.Settings.isMobile)

            wrapMode: Text.Wrap

            Layout.fillWidth: true
        }
        Shared.ReplyBlock {}
        TextEdit {
            id: textEdit
            text: textData.data.content + (web.visible ? "" : _background.textPadding)

            Connections {
                id: conns

                target: textData.data
                function onContentChanged() {
                    textMessageRoot.isWide = textData.model.format(textData.key, textEdit.textDocument, textEdit, textEdit.isEmojiOnly)
                }
            }
            Component.onCompleted: conns.onContentChanged()

            readonly property var isEmoji: /^(\u00a9|\u00ae|[\u2000-\u3300]|\ud83c[\ud000-\udfff]|\ud83d[\ud000-\udfff]|\ud83e[\ud000-\udfff])+$/
            readonly property bool isEmojiOnly: isEmoji.test(textData.data.content)

            readOnly: true
            selectByMouse: !Kirigami.Settings.isMobile
            wrapMode: Text.Wrap

            color: Kirigami.Theme.textColor
            selectedTextColor: Kirigami.Theme.highlightedTextColor
            selectionColor: Kirigami.Theme.highlightColor

            function clamp() {
                if (web.visible) return

                const l = length - _background.textPadding.length
                if (selectionEnd >= l && selectionStart >= l) {
                    select(0, 0)
                } else if (selectionEnd >= l) {
                    select(selectionStart, l)
                } else if (selectionStart >= l) {
                    select(l, selectionEnd)
                }
            }

            onSelectionStartChanged: clamp()
            onSelectionEndChanged: clamp()

            onLinkActivated: (mu) => LinkActivation.handle(mu, globalUserDataSheet, tClient)

            HoverHandler {
                acceptedButtons: Qt.NoButton
                cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.IBeamCursor
            }

            Layout.fillWidth: true
        }

        Shared.WebPageBlock {
            id: web
            Layout.bottomMargin: _background.inlineFooter.height
        }
    }

    property bool isWide: false
    Layout.maximumWidth: isWide ? textEdit.implicitWidth + leftPadding + rightPadding : del.recommendedSize
    Layout.fillWidth: isWide
}
