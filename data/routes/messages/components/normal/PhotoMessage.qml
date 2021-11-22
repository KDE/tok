// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtGraphicalEffects 1.15
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.12 as Kirigami
import org.kde.Tok 1.0 as Tok

import "qrc:/components" as Components
import "../link_activation.mjs" as LinkActivation

import "../shared" as Shared

QQC2.Control {
    id: photoRoot

    topPadding: 0
    bottomPadding: 0
    leftPadding: tailSize
    rightPadding: 0

    readonly property int tailSize: Kirigami.Units.largeSpacing

    property bool isWide: false
    Layout.maximumWidth: isWide ? -1 : del.recommendedSize
    Layout.fillWidth: isWide

    background: MessageBackground {
        id: _background
        tailSize: photoRoot.tailSize

        anchors.fill: parent
    }

    contentItem: ColumnLayout {
        Shared.ReplyBlock {
            Layout.topMargin: Kirigami.Units.smallSpacing
            Layout.bottomMargin: Kirigami.Units.smallSpacing
            Layout.leftMargin: Kirigami.Units.largeSpacing
            Layout.rightMargin: Kirigami.Units.largeSpacing
        }
        Image {
            id: image

            source: imageData.data.imageURL
            sourceSize: imageData.data.imageDimensions

            Rectangle {
                color: Kirigami.Theme.backgroundColor
                anchors.fill: image
                visible: image.status !== Image.Ready

                Loader {
                    anchors.fill: parent
                    active: image.status !== Image.Ready
                    sourceComponent: Image {
                        anchors.fill: parent
                        source: imageData.data.imageMinithumbnail

                        layer.enabled: true
                        layer.effect: FastBlur {
                            cached: true
                            radius: 32
                        }
                    }
                }
            }

            readonly property real ratio: width / sourceSize.width
            Layout.preferredHeight: sourceSize.height * image.ratio
            Layout.fillWidth: true

            Accessible.name: i18n("Photo message.")

            smooth: true
            mipmap: true

            HoverHandler {
                cursorShape: Qt.PointingHandCursor
            }
            TapHandler {
                onTapped: imagePopup.open()
            }

            Components.ImagePopup {
                id: imagePopup
                key: [del.mChatID, del.mID]
            }

            Tok.RelationalListener {
                id: imageData

                model: tClient.messagesStore
                key: [del.mChatID, del.mID]
                shape: QtObject {
                    required property string imageURL
                    required property string imageCaption
                    required property size imageDimensions
                    required property string imageMinithumbnail
                }
            }

            QQC2.Label {
                text: messageData.data.timestamp

                Kirigami.Theme.colorSet: Kirigami.Theme.Complementary
                Kirigami.Theme.inherit: false

                font.pointSize: -1
                font.pixelSize: Kirigami.Units.gridUnit * (2/3)

                padding: Kirigami.Units.smallSpacing
                leftPadding: Math.floor(Kirigami.Units.smallSpacing*(3/2))
                rightPadding: Math.floor(Kirigami.Units.smallSpacing*(3/2))

                visible: !textEdit.visible

                anchors {
                    bottom: parent.bottom
                    right: parent.right
                    margins: Kirigami.Units.largeSpacing
                }
                background: Rectangle {
                    color: Kirigami.Theme.backgroundColor
                    opacity: 0.7
                    radius: 3
                }
            }

            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    color: "red"
                    radius: 4
                    width: image.width
                    height: image.height
                }
            }
        }
        TextEdit {
            id: textEdit
            text: imageData.data.imageCaption + _background.textPadding
            visible: imageData.data.imageCaption != ""

            topPadding: Kirigami.Units.smallSpacing
            bottomPadding: Kirigami.Units.largeSpacing
            leftPadding: Kirigami.Units.largeSpacing
            rightPadding: Kirigami.Units.largeSpacing

            Connections {
                id: conns

                target: imageData.data
                function onImageCaptionChanged() {
                    photoRoot.isWide = imageData.model.format(imageData.key, textEdit.textDocument, textEdit, textEdit.isEmojiOnly)
                }
            }
            Component.onCompleted: conns.onImageCaptionChanged()

            readOnly: true
            selectByMouse: !Kirigami.Settings.isMobile
            wrapMode: Text.Wrap

            color: Kirigami.Theme.textColor
            selectedTextColor: Kirigami.Theme.highlightedTextColor
            selectionColor: Kirigami.Theme.highlightColor

            function clamp() {
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
    }

}
