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
import Qt.labs.folderlistmodel 2.15
import Qt.labs.platform 1.1

QQC2.Drawer {
    id: uploadDialog

    edge: Qt.BottomEdge
    width: QQC2.Overlay.overlay.width
    height: Math.floor(0.9 * QQC2.Overlay.overlay.height)
    interactive: {
        if (!visible)
            return false
        
        switch (uploadDialog.currentPage) {
        case "photos":
            return photosView.atYBeginning
        case "videos":
            return videosView.atYBeginning
        case "music":
            return musicView.atYBeginning
        case "files":
            return foldersView.atYBeginning
        }
    }

    property string chatID: ""

    // "photos" | "videos" | "music" | "files"
    property string currentPage: "photos"

    property string currentType: ""
    property url currentUrl: ""

    function pick() {
        visible = true
        texty.text = ""
        uploadDialog.currentType = ""
        uploadDialog.currentUrl = ""
        uploadDialog.currentPage = "photos"
        photosView.positionViewAtBeginning()
        videosView.positionViewAtBeginning()
        musicView.positionViewAtBeginning()
        foldersView.positionViewAtBeginning()
    }

    component Emblem : QQC2.Control {
        background: Rectangle {
            color: Kirigami.Theme.highlightColor
            radius: height/2
        }
        contentItem: Kirigami.Icon {
            color: Kirigami.Theme.highlightedTextColor
            source: "dialog-ok"
            implicitHeight: 16
            implicitWidth: 16
        }
    }

    ColumnLayout {
        anchors.fill: parent

        GlobalComponents.Header {
            Layout.fillWidth: true

            Kirigami.Heading {
                level: 4
                text: i18nc("page title", "Upload…")
                Layout.margins: Kirigami.Units.largeSpacing
            }
        }

        GridView {
            id: photosView

            clip: true
            visible: uploadDialog.currentPage === "photos"

            Layout.fillWidth: true
            Layout.fillHeight: true

            model: FolderListModel {
                showDirs: false
                sortField: FolderListModel.Time
                folder: StandardPaths.writableLocation(StandardPaths.PicturesLocation)
                nameFilters: ["*.png", "*.jpg", "*.jpeg"]
            }

            cellWidth: width/3
            cellHeight: cellWidth

            delegate: Item {
                id: del

                width: photosView.cellWidth
                height: photosView.cellHeight

                required property url fileUrl

                TapHandler {
                    onTapped: {
                        uploadDialog.currentUrl = del.fileUrl
                        uploadDialog.currentType = "image"
                    }
                }

                Image {
                    fillMode: Image.PreserveAspectCrop
                    asynchronous: true

                    anchors.centerIn: parent
                    source: del.fileUrl

                    width: photosView.cellWidth - 4
                    height: photosView.cellHeight - 4

                    sourceSize.width: width
                    sourceSize.height: height
                }


                Emblem {
                    anchors {
                        top: parent.top
                        right: parent.right
                        margins: Kirigami.Units.largeSpacing
                    }
                    visible: del.fileUrl === uploadDialog.currentUrl
                }
            }
        }

        ListView {
            id: videosView

            clip: true
            visible: uploadDialog.currentPage === "videos"

            Layout.fillWidth: true
            Layout.fillHeight: true

            model: FolderListModel {
                showDirs: false
                sortField: FolderListModel.Time
                folder: StandardPaths.writableLocation(StandardPaths.MoviesLocation)
                nameFilters: ["*.mp4"]
            }

            delegate: Kirigami.BasicListItem {
                id: del

                width: videosView.width

                required property url fileUrl
                required property string fileBaseName
                text: fileBaseName

                onClicked: {
                    uploadDialog.currentUrl = del.fileUrl
                    uploadDialog.currentType = "video"
                }

                trailing: Item {
                    width: emblem.width
                    height: emblem.height

                    Emblem {
                        id: emblem

                        anchors {
                            right: parent.right
                            verticalCenter: parent.verticalCenter
                        }
                        visible: del.fileUrl === uploadDialog.currentUrl
                    }
                }
            }
        }

        ListView {
            id: musicView

            clip: true
            visible: uploadDialog.currentPage === "music"

            Layout.fillWidth: true
            Layout.fillHeight: true

            model: FolderListModel {
                showDirs: false
                sortField: FolderListModel.Time
                folder: StandardPaths.writableLocation(StandardPaths.MusicLocation)
                nameFilters: ["*.mp3"]
            }

            delegate: Kirigami.BasicListItem {
                id: del

                width: musicView.width

                required property string fileBaseName
                required property url fileUrl
                text: fileBaseName

                onClicked: {
                    uploadDialog.currentUrl = del.fileUrl
                    uploadDialog.currentType = "audio"
                }

                trailing: Item {
                    width: emblem.width
                    height: emblem.height

                    Emblem {
                        id: emblem

                        anchors {
                            right: parent.right
                            verticalCenter: parent.verticalCenter
                        }
                        visible: del.fileUrl === uploadDialog.currentUrl
                    }
                }
            }
        }

        ListView {
            id: foldersView

            clip: true
            visible: uploadDialog.currentPage === "files"

            Layout.fillWidth: true
            Layout.fillHeight: true

            model: FolderListModel {
                id: browserModel
                showDirs: true
                showDirsFirst: true
                showDotAndDotDot: true
                sortField: FolderListModel.Name
                folder: StandardPaths.writableLocation(StandardPaths.HomeLocation)
            }

            delegate: Kirigami.BasicListItem {
                id: del

                width: foldersView.width

                leading: Item {
                    width: 22

                    Kirigami.Icon {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left

                        source: del.fileIsDir ? "folder" : Tok.Utils.fileIcon(del.fileUrl)
                        width: 22
                        height: 22
                    }
                }
                reserveSpaceForSubtitle: true

                required property string fileName
                required property url fileUrl
                required property bool fileIsDir
                text: fileName
                onClicked: {
                    if (fileIsDir) {
                        browserModel.folder = del.fileUrl
                    } else {
                        uploadDialog.currentUrl = del.fileUrl
                        uploadDialog.currentType = "file"
                    }
                }

                trailing: Item {
                    width: emblem.width
                    height: emblem.height

                    Emblem {
                        id: emblem

                        anchors {
                            right: parent.right
                            verticalCenter: parent.verticalCenter
                        }
                        visible: del.fileUrl === uploadDialog.currentUrl
                    }
                }
            }
        }

        RowLayout {
            UploadComponents.RichTextArea {
                id: texty
            }
            QQC2.Button {
                text: i18nc("button", "Send")
                enabled: uploadDialog.currentUrl !== ""
                onClicked: {
                    tClient.messagesModel(uploadDialog.chatID).sendAttachment(texty.textDocument, uploadDialog.currentUrl, uploadDialog.currentType)
                    uploadDialog.close()
                }
            }

            Layout.fillWidth: true
        }

        QQC2.ToolBar {
            Layout.fillWidth: true
            position: QQC2.ToolBar.Footer

            contentItem: ColumnLayout {
                component PageDelegate : ColumnLayout {
                    id: del

                    required property string icon
                    required property string label
                    required property string page

                    Kirigami.Icon {
                        source: del.icon
                        color: uploadDialog.currentPage === del.page ? Kirigami.Theme.highlightColor : Kirigami.Theme.textColor

                        Layout.preferredWidth: 22
                        Layout.preferredHeight: 22

                        Layout.alignment: Qt.AlignHCenter
                    }
                    QQC2.Label {
                        text: del.label
                        color: uploadDialog.currentPage === del.page ? Kirigami.Theme.highlightColor : Kirigami.Theme.textColor
                        font.weight: uploadDialog.currentPage === del.page ? Font.Black : Kirigami.Theme.defaultFont.weight

                        horizontalAlignment: Text.AlignHCenter
                        Layout.fillWidth: true
                    }

                    TapHandler {
                        onTapped: uploadDialog.currentPage = del.page
                    }

                    Layout.preferredWidth: 1
                    Layout.fillWidth: true
                }

                RowLayout {
                    Layout.fillWidth: true

                    PageDelegate {
                        icon: "photo"
                        label: i18nc("page title in switcher; keep small as possible", "Photos")
                        page: "photos"
                    }
                    PageDelegate {
                        icon: "videoclip-amarok"
                        label: i18nc("page title in switcher; keep small as possible", "Videos")
                        page: "videos"
                    }
                    PageDelegate {
                        icon: "folder-music-symbolic"
                        label: i18nc("page title in switcher; keep small as possible", "Music")
                        page: "music"
                    }
                    PageDelegate {
                        icon: "document-open"
                        label: i18nc("page title in switcher; keep small as possible", "Files")
                        page: "files"
                    }
                }
            }
        }
    }
}