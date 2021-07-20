// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

QQC2.Control {
    id: fileMessageRoot

    topPadding: Kirigami.Units.largeSpacing
    bottomPadding: Kirigami.Units.largeSpacing
    leftPadding: Kirigami.Units.largeSpacing+2+tailSize
    rightPadding: Kirigami.Units.largeSpacing+2

    readonly property int tailSize: Kirigami.Units.largeSpacing

    onStateChanged: {
        if (state !== "downloaded") return

        if (!settings.userHasDownloadedFile && !Kirigami.Settings.isMobile) {
            settings.userHasDownloadedFile = true
            didYouKnow.visible = true
        }
    }

    states: [
        State {
            name: "downloaded"
            when: downloadData.data.mLocalFileDownloadCompleted

            PropertyChanges {
                target: downloadButton
                icon.name: "document-open"
                QQC2.ToolTip.text: i18nc("tooltip for a button on a message; offers ability to open its downloaded file with an appropriate application", "Open File")
                onClicked: Qt.openUrlExternally("file://" + downloadData.data.mLocalFilePath)
            }
        },
        State {
            name: "downloading"
            when: downloadData.data.mLocalFileIsDownloading

            PropertyChanges {
                target: sizeLabel
                text: i18nc("file download progress", "%1 / %2", Tok.Utils.humanSize(downloadData.data.mLocalFileDownloadedSize), Tok.Utils.humanSize(downloadData.data.mExpectedFileSize))
            }
            PropertyChanges {
                target: downloadButton
                icon.name: "media-playback-stop"

                QQC2.ToolTip.text: i18nc("tooltip for a button on a message; offers ability to open its downloaded file with an appropriate application", "Stop Download")
                onClicked: tClient.fileMangler.stopDownloadingFile(fileData.data.fileID)
            }
        },
        State {
            name: "raw"
            when: true

            PropertyChanges {
                target: downloadButton

                onClicked: {
                    tClient.fileMangler.downloadFile(fileData.data.fileID)
                }
            }
        }
    ]

    Accessible.name: `${userData.data.name} uploaded a file: ${fileData.data.fileName}`

    Tok.RelationalListener {
        id: downloadData

        model: tClient.fileMangler
        key: fileData.data.fileID
        shape: QtObject {
            required property int mFileSize
            required property int mExpectedFileSize
            required property string mLocalFilePath
            required property bool mLocalFileDownloadable
            required property bool mLocalFileDeletable
            required property bool mLocalFileIsDownloading
            required property bool mLocalFileDownloadCompleted
            required property int mLocalFileDownloadedSize
        }
    }

    Tok.RelationalListener {
        id: fileData

        model: tClient.messagesStore
        key: [del.mChatID, del.mID]
        shape: QtObject {
            required property string fileName
            required property string fileCaption
            required property string fileID
            required property string fileIcon
            required property string fileSizeHuman
        }
    }

    background: MessageBackground {
        id: _background
        tailSize: fileMessageRoot.tailSize
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
            bottomPadding: fileData.data.fileCaption !== "" ? Kirigami.Units.largeSpacing : Kirigami.Units.largeSpacing+Kirigami.Units.smallSpacing
            contentItem: RowLayout {
                Kirigami.Icon {
                    id: ikon

                    QQC2.ToolTip {
                        id: didYouKnow

                        parent: ikon
                        timeout: 30 * 1000

                        padding: Kirigami.Units.largeSpacing

                        contentItem: Row {
                            spacing: 4

                            Kirigami.Icon {
                                source: "data-information"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            QQC2.Label {
                                width: Kirigami.Units.gridUnit * 12
                                wrapMode: Text.Wrap
                                text: i18nc("explanatory popup notification", "You can drag and drop the icon on downloaded files into other applications.")
                            }
                        }
                    }

                    source: fileData.data.fileIcon
                    fallback: "unknown"

                    Drag.active: dragArea.drag.active
                    Drag.dragType: Drag.Automatic
                    Drag.supportedActions: Qt.CopyAction
                    Drag.mimeData: {
                        "text/uri-list": "file://"+downloadData.data.mLocalFilePath
                    }

                    MouseArea {
                        id: dragArea
                        anchors.fill: parent

                        enabled: fileMessageRoot.state === "downloaded"
                        drag.target: parent
                        cursorShape: {
                            if (pressed) {
                                return Qt.ClosedHandCursor
                            } else if (fileMessageRoot.state === "downloaded") {
                                return Qt.OpenHandCursor
                            }
                            return Qt.ArrowCursor
                        }

                        onPressed: parent.grabToImage(function(result) {
                            parent.Drag.imageSource = result.url
                        })
                    }
                }
                ColumnLayout {
                    Layout.alignment: Qt.AlignVCenter
                    Layout.fillWidth: true

                    spacing: 0

                    QQC2.Label {
                        text: fileData.data.fileName
                        wrapMode: Text.Wrap

                        Layout.fillWidth: true
                    }
                    QQC2.Label {
                        id: sizeLabel

                        text: fileData.data.fileSizeHuman
                        opacity: 0.7

                        Layout.fillWidth: true
                    }
                }

                QQC2.Button {
                    id: downloadButton

                    icon.name: "download"

                    Kirigami.Theme.backgroundColor: del.nestedButtonColor

                    QQC2.ToolTip.text: i18nc("tooltip for a button on a message; offers ability to download its file", "Download")
                    QQC2.ToolTip.visible: hovered

                    Layout.leftMargin: Kirigami.Units.smallSpacing
                    Layout.alignment: Qt.AlignVCenter
                }
            }

            Layout.fillWidth: true
        }
        QQC2.Label {
            text: fileData.data.fileCaption + _background.textPadding
            wrapMode: Text.Wrap
            visible: fileData.data.fileCaption != ""

            Layout.fillWidth: true
        }
    }

    Layout.maximumWidth: del.recommendedSize
}