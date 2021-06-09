import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtGraphicalEffects 1.15
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.12 as Kirigami
import org.kde.Tok 1.0 as Tok

import "qrc:/components" as Components

QQC2.Control {
    id: videoRoot

    topPadding: 0
    bottomPadding: 0
    leftPadding: tailSize
    rightPadding: 0

    readonly property int tailSize: Kirigami.Units.largeSpacing

    Layout.maximumWidth: del.recommendedSize

    background: MessageBackground {
        id: _background
        tailSize: videoRoot.tailSize

        anchors.fill: parent
    }

    contentItem: ColumnLayout {
        ReplyBlock {}
        Image {
            id: video

            source: videoData.data.videoThumbnail

            readonly property real ratio: width / implicitWidth
            Layout.preferredHeight: video.implicitHeight * video.ratio
            Layout.fillWidth: true

            Accessible.name: i18n("Photo message.")

            smooth: true
            mipmap: true

            HoverHandler {
                cursorShape: Qt.PointingHandCursor
            }
            TapHandler {
                onTapped: tClient.messagesStore.openVideo(del.mChatID, del.mID)
            }

            Rectangle {
                id: scrim

                anchors.fill: icon
                anchors.margins: -Kirigami.Units.largeSpacing

                color: Qt.rgba(0, 0, 0, 0.3)

                radius: width / 2
            }

            Kirigami.Icon {
                id: icon

                anchors.centerIn: parent

                source: "media-playback-start"
                Kirigami.Theme.textColor: "white"
            }

            Tok.RelationalListener {
                id: videoData

                model: tClient.messagesStore
                key: [del.mChatID, del.mID]
                shape: QtObject {
                    required property size videoSize
                    required property string videoThumbnail
                    required property string videoCaption
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
                    width: video.width
                    height: video.height
                }
            }
        }
        TextEdit {
            id: textEdit
            text: videoData.data.videoCaption + paddingT
            visible: videoData.data.videoCaption != ""

            topPadding: Kirigami.Units.smallSpacing
            bottomPadding: Kirigami.Units.largeSpacing
            leftPadding: Kirigami.Units.largeSpacing
            rightPadding: Kirigami.Units.largeSpacing

            Connections {
                id: conns

                target: videoData.data
                function onVideoCaptionChanged() {
                    videoData.model.format(videoData.key, textEdit.textDocument, textEdit, textEdit.isEmojiOnly)
                }
            }
            Component.onCompleted: conns.onVideoCaptionChanged()

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
