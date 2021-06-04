import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtGraphicalEffects 1.15
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

import "qrc:/components" as Components

QQC2.Control {
    id: audioMessageRoot

    topPadding: Kirigami.Units.largeSpacing
    bottomPadding: Kirigami.Units.largeSpacing
    leftPadding: Kirigami.Units.largeSpacing+tailSize
    rightPadding: Kirigami.Units.largeSpacing

    readonly property int tailSize: Kirigami.Units.largeSpacing

    Accessible.name: `${userData.data.name} sent audio: ${audioData.data.audioTitle || audioData.data.audioFilename}`

    Tok.RelationalListener {
        id: audioData

        model: tClient.messagesStore
        key: [del.mChatID, del.mID]
        shape: QtObject {
            required property string audioCaption
            required property int audioDuration
            required property string audioTitle
            required property string audioPerformer
            required property string audioFilename
            required property string audioMimetype
            required property string audioSmallThumbnail
            required property string audioLargeThumbnail
            required property string audioFileID
        }
    }

    background: MessageBackground {
        id: _background
        tailSize: audioMessageRoot.tailSize
    }
    contentItem: ColumnLayout {
        QQC2.Label {
            text: userData.data.name
            color: Kirigami.NameUtils.colorsFromString(text)

            visible: del.separateFromPrevious && !(del.isOwnMessage && Kirigami.Settings.isMobile)

            wrapMode: Text.Wrap

            Layout.fillWidth: true
        }
        ReplyBlock {}
        QQC2.Control {
            padding: Kirigami.Units.largeSpacing
            contentItem: RowLayout {
                Rectangle {
                    visible: !image.visible

                    radius: width/2

                    color: Kirigami.Theme.focusColor

                    Kirigami.Icon {
                        anchors.centerIn: parent

                        source: "media-playback-start"
                        Kirigami.Theme.textColor: "white"
                    }

                    Layout.preferredWidth: 40
                    Layout.preferredHeight: 40
                }
                Image {
                    id: image

                    Layout.preferredWidth: 40
                    Layout.preferredHeight: 40

                    source: audioData.data.audioSmallThumbnail
                    visible: status == Image.Ready

                    Rectangle {
                        anchors.fill: parent

                        color: Qt.rgba(0, 0, 0, 0.5)
                    }

                    Kirigami.Icon {
                        anchors.centerIn: parent

                        source: "media-playback-start"
                        Kirigami.Theme.textColor: "white"
                    }

                    layer.enabled: true
                    layer.effect: OpacityMask {
                        maskSource: Rectangle {
                            color: "red"
                            radius: width/2
                            width: image.width
                            height: image.height
                        }
                    }
                }
                ColumnLayout {
                    spacing: 0

                    Layout.leftMargin: Kirigami.Units.largeSpacing

                    QQC2.Label {
                        text: audioData.data.audioTitle || audioData.data.audioFilename
                        wrapMode: Text.Wrap

                        Layout.fillWidth: true
                    }
                    QQC2.Label {
                        text: audioData.data.audioPerformer
                        visible: text != ""
                        wrapMode: Text.Wrap
                        opacity: 0.7

                        Layout.fillWidth: true
                    }

                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter
                }
                HoverHandler {
                    cursorShape: Qt.PointingHandCursor
                }
                TapHandler {
                    onTapped: {
                        if (audioData.data.audioLargeThumbnail != "") {
                            tClient.fileMangler.downloadFile(audioData.data.audioLargeThumbnail).then((url) => {
                                Components.AudioPlayer.thumbnail = "file://"+url
                            })
                        } else {
                            Components.AudioPlayer.thumbnail = ""
                        }
                        tClient.fileMangler.downloadFile(audioData.data.audioFileID).then((url) => {
                            Components.AudioPlayer.source = "file://"+url
                            Components.AudioPlayer.play()
                        })
                    }
                }
            }

            Layout.fillWidth: true
        }
        QQC2.Label {
            text: audioData.data.audioCaption + paddingT
            wrapMode: Text.Wrap
            visible: audioData.data.audioCaption != ""

            readonly property string paddingT: " ".repeat(Math.ceil(_background.timestamp.implicitWidth / _background.dummy.implicitWidth)) + "⠀"

            Layout.fillWidth: true
        }
    }

    Layout.maximumWidth: del.recommendedSize
}
