import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtGraphicalEffects 1.15
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.12 as Kirigami
import QtMultimedia 5.15
import org.kde.Tok 1.0 as Tok

import "qrc:/components" as Components

QQC2.Control {
    id: photoRoot

    topPadding: 0
    bottomPadding: 0
    leftPadding: tailSize
    rightPadding: 0

    readonly property int tailSize: Kirigami.Units.largeSpacing

    Layout.maximumWidth: del.recommendedSize

    background: MessageBackground {
        id: _background
        tailSize: photoRoot.tailSize

        anchors.fill: parent
    }

    Tok.RelationalListener {
        id: gifData

        model: tClient.messagesStore
        key: [del.mChatID, del.mID]
        shape: QtObject {
            required property string animationFileID
            onAnimationFileIDChanged: {
                tClient.fileMangler.downloadFile(animationFileID).then((url) => {
                    vidya.source = "file://"+url
                })
            }
            required property string animationThumbnail
            required property string animationCaption
        }
    }

    contentItem: ColumnLayout {
        ReplyBlock {
            Layout.topMargin: Kirigami.Units.smallSpacing
            Layout.bottomMargin: Kirigami.Units.smallSpacing
            Layout.leftMargin: Kirigami.Units.largeSpacing
            Layout.rightMargin: Kirigami.Units.largeSpacing
        }
        Image {
            id: image

            source: gifData.data.animationThumbnail

            readonly property real ratio: width / implicitWidth
            Layout.preferredHeight: image.implicitHeight * image.ratio
            Layout.fillWidth: true

            Accessible.name: i18n("GIF message.")

            smooth: true
            mipmap: true

            Video {
                id: vidya

                autoPlay: true
                loops: MediaPlayer.Infinite

                anchors.fill: parent
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
            text: gifData.data.animationCaption + paddingT
            visible: gifData.data.animationCaption != ""

            topPadding: Kirigami.Units.smallSpacing
            bottomPadding: Kirigami.Units.largeSpacing
            leftPadding: Kirigami.Units.largeSpacing
            rightPadding: Kirigami.Units.largeSpacing

            Connections {
                id: conns

                target: gifData.data
                function onAnimationCaptionChanged() {
                    gifData.model.format(gifData.key, textEdit.textDocument, textEdit, textEdit.isEmojiOnly)
                }
            }
            Component.onCompleted: conns.onAnimationCaptionChanged()

            readonly property string paddingT: " ".repeat(Math.ceil(_background.timestamp.implicitWidth / _background.dummy.implicitWidth)) + "⠀"

            readOnly: true
            selectByMouse: !Kirigami.Settings.isMobile
            wrapMode: Text.Wrap

            color: Kirigami.Theme.textColor
            selectedTextColor: Kirigami.Theme.highlightedTextColor
            selectionColor: Kirigami.Theme.highlightColor

            function clamp() {
                const l = length - paddingT.length
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

            onLinkActivated: (mu) => {
                Qt.openUrlExternally(mu)
            }

            HoverHandler {
                acceptedButtons: Qt.NoButton
                cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.IBeamCursor
            }

            Layout.fillWidth: true
        }
    }

}
