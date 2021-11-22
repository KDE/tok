// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

import "../shared" as Shared

QQC2.Control {
    id: fileMessageRoot

    topPadding: Kirigami.Units.smallSpacing
    bottomPadding: Kirigami.Units.smallSpacing
    leftPadding: Kirigami.Units.largeSpacing+tailSize
    rightPadding: Kirigami.Units.largeSpacing

    states: [
        State {
            name: "downloaded"
            when: downloadData.data.mLocalFileDownloadCompleted

            PropertyChanges {
                target: downloadButton
                icon.name: "document-open"
                text: i18nc("tooltip for a button on a message; offers ability to open its downloaded file with an appropriate application", "Open File")
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

                text: i18nc("tooltip for a button on a message; offers ability to open its downloaded file with an appropriate application", "Stop Download")
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

    readonly property int tailSize: Kirigami.Units.largeSpacing

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
            required property string fileSizeHuman
        }
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
        QQC2.Button {
            id: downloadButton

            text: i18nc("a button", "Download")
            icon.name: "download"

            Layout.leftMargin: Kirigami.Units.smallSpacing
            Layout.alignment: Qt.AlignVCenter
        }
        QQC2.Label {
            text: fileData.data.fileCaption
            wrapMode: Text.Wrap
            visible: fileData.data.fileCaption != ""

            Layout.fillWidth: true
        }
    }

    Layout.fillWidth: true
}