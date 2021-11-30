// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.10
import org.kde.Tok 1.0 as Tok
import org.kde.kirigami 2.15 as Kirigami
import QtQuick.Dialogs 1.3
import "qrc:/components" as GlobalComponents
import QtMultimedia 5.15
import "qrc:/components/upload" as UploadComponents

Dialog {
    id: _uploadDialogDesktop

    title: i18nc("dialog title", "Upload")
    width: Math.max(contentItem.implicitWidth, Kirigami.Units.gridUnit * 25)
    height: Math.max(contentItem.implicitHeight, Kirigami.Units.gridUnit * 10)

    property string chatID: ""

    property string source: ""
    onSourceChanged: sourceData = Tok.Utils.fileData(source)
    property var sourceData: {
        return {
            "size": 0,
            "name": "",
            "type": "",
            "icon": "",
        }
    }

    function pick() {
        Tok.Utils.pickFile(i18nc("Dialog title", "Upload"), "none").then((url) => {
            _uploadDialogDesktop.source = url
            _uploadDialogDesktop.open()
        })
    }

    component ButtonsRow : RowLayout {
        id: _buttonsRow

        required property Item textArea
        property bool forceFile: false

        Layout.fillWidth: true
        Item { Layout.fillWidth: true }
        QQC2.Button {
            text: i18nc("button", "Cancel")
            onClicked: {
                _uploadDialogDesktop.close()
                _uploadDialogDesktop.sourceData = {
                    "size": 0,
                    "name": "",
                    "type": "",
                    "icon": "",
                }
            }
        }
        QQC2.Button {
            text: i18nc("button", "Send")
            onClicked: {
                tClient.messagesModel(_uploadDialogDesktop.chatID).sendAttachment(_buttonsRow.textArea.textDocument, _uploadDialogDesktop.source, _buttonsRow.forceFile ? "file" : _uploadDialogDesktop.sourceData.type)
                _uploadDialogDesktop.close()
            }
        }
    }

contentItem: QQC2.Control {
    Component {
        id: imageDelegate

        ColumnLayout {
            Item {
                Layout.preferredHeight: image.implicitHeight * image.ratio
                Layout.fillWidth: true

                Rectangle {
                    color: "white"
                    anchors.fill: parent
                }
                Image {
                    id: image
                    source: _uploadDialogDesktop.source
                    fillMode: Image.PreserveAspectCrop
                    anchors.fill: parent

                    readonly property real ratio: width / implicitWidth
                }
            }
            RowLayout {
                Kirigami.Heading {
                    level: 4
                    text: _uploadDialogDesktop.sourceData.name

                    verticalAlignment: Text.AlignVCenter
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
            }
            QQC2.CheckBox {
                id: compressionBox
                text: i18n("Compress image")
                checked: true
            }

            UploadComponents.RichTextArea { id: richTextArea }
            ButtonsRow {
                textArea: richTextArea
                forceFile: !compressionBox.checked
            }
        }
    }
    Component {
        id: videoDelegate
        ColumnLayout {
            Video {
                id: video
                source: _uploadDialogDesktop.source
                autoPlay: true
                volume: 0.0

                onPlaybackStateChanged: pause()

                Layout.preferredHeight: video.metaData.resolution.height * video.ratio
                readonly property real ratio: width / video.metaData.resolution.width

                Layout.fillWidth: true
            }
            RowLayout {
                Kirigami.Heading {
                    level: 4
                    text: _uploadDialogDesktop.sourceData.name

                    verticalAlignment: Text.AlignVCenter
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
                QQC2.Label {
                    text: Tok.Utils.humanSize(_uploadDialogDesktop.sourceData.size)

                    verticalAlignment: Text.AlignVCenter
                    Layout.fillHeight: true
                }
            }

            UploadComponents.RichTextArea { id: richTextArea }
            ButtonsRow { textArea: richTextArea }
        }
    }
    Component {
        id: fileDelegate
        ColumnLayout {
            RowLayout {
                Kirigami.Icon {
                    id: ikon

                    source: _uploadDialogDesktop.sourceData.icon
                    fallback: "unknown"
                }
                Column {
                    Kirigami.Heading {
                        level: 2
                        text: _uploadDialogDesktop.sourceData.name
                    }
                    QQC2.Label {
                        text: Tok.Utils.humanSize(_uploadDialogDesktop.sourceData.size)
                    }
                }
            }

            UploadComponents.RichTextArea { id: richTextArea }
            ButtonsRow { textArea: richTextArea }
        }
    }
    Component {
        id: audioDelegate
        ColumnLayout {
            RowLayout {
                Kirigami.Icon {
                    id: ikon

                    source: _uploadDialogDesktop.sourceData.icon
                    fallback: "unknown"
                }
                Column {
                    Kirigami.Heading {
                        level: 2
                        text: _uploadDialogDesktop.sourceData.name
                    }
                    QQC2.Label {
                        text: Tok.Utils.humanSize(_uploadDialogDesktop.sourceData.size)
                    }
                }
            }

            UploadComponents.RichTextArea { id: richTextArea }
            ButtonsRow { textArea: richTextArea }
        }
    }
    background: Rectangle {
        Kirigami.Separator {
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
        }
        color: Kirigami.Theme.backgroundColor
    }
    contentItem: Loader {
        sourceComponent: {
            if (_uploadDialogDesktop.sourceData.type == "") {
                return null
            }

            const sources = {
                "image": imageDelegate,
                "video": videoDelegate,
                "file": fileDelegate,
                "audio": audioDelegate,
            }

            return sources[_uploadDialogDesktop.sourceData.type]
        }
    }
}
}
