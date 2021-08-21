import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

import QtQuick.Dialogs 1.0 as Dialogues

import "qrc:/components" as GlobalComponents

MediaListView {
    id: mediaView

    title: i18n("Music")
    model: tClient.searchMessagesModel({
        "chatID": chatData.key,
        "kind": "audios",
    })
    isWide: true

    delegate: QQC2.Control {
        id: del
    
        horizontalPadding: Kirigami.Units.smallSpacing*2
        verticalPadding: Kirigami.Units.smallSpacing
        required property string messageID

        width: parent.width

        Tok.RelationalListener {
            id: audioData

            model: tClient.messagesStore
            key: [chatData.key, del.messageID]
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

        HoverHandler {
            cursorShape: Qt.PointingHandCursor
        }

        contentItem: RowLayout {
            Image {
                fillMode: Image.PreserveAspectCrop
                source: audioData.data.audioLargeThumbnail
                visible: status === Image.Ready

                sourceSize.width: width
                sourceSize.height: height

                Layout.preferredHeight: 40
                Layout.preferredWidth: 40
            }
            ColumnLayout {
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
            }

            QQC2.Button {
                icon.name: "media-playback-start"
                onClicked: {
                    if (GlobalComponents.AudioPlayer.audioID == audioData.data.audioFileID) {
                        if (GlobalComponents.AudioPlayer.playbackState == Audio.PlayingState)
                            GlobalComponents.AudioPlayer.pause()
                        else
                            GlobalComponents.AudioPlayer.play()

                        return
                    }

                    if (audioData.data.audioLargeThumbnail != "") {
                        tClient.fileMangler.downloadFile(audioData.data.audioLargeThumbnail).then((url) => {
                            GlobalComponents.AudioPlayer.thumbnail = "file://"+url
                        })
                    } else {
                        GlobalComponents.AudioPlayer.thumbnail = ""
                    }
                    tClient.fileMangler.downloadFile(audioData.data.audioFileID).then((url) => {
                        GlobalComponents.AudioPlayer.source = "file://"+url
                        GlobalComponents.AudioPlayer.audioID = audioData.data.audioFileID
                        GlobalComponents.AudioPlayer.play()
                    })
                }
            }
        }
    }
}
