import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

import QtQuick.Dialogs 1.0 as Dialogues

import "qrc:/components" as GlobalComponents

MediaView {
    id: mediaView

    title: i18n("Photos")
    model: tClient.searchMessagesModel({
        "chatID": chatData.key,
        "kind": "photos",
    })
    delegate: Item {
        id: del
        required property string messageID

        width: mediaView.cellWidth
        height: mediaView.cellHeight

        HoverHandler {
            cursorShape: Qt.PointingHandCursor
        }
        TapHandler {
            onTapped: imagePopup.open()
        }

        GlobalComponents.ImagePopup {
            id: imagePopup
            key: [chatData.key, del.messageID]
        }

        Tok.RelationalListener {
            id: imageData

            model: tClient.messagesStore
            key: [chatData.key, del.messageID]
            shape: QtObject {
                required property string imageURL
            }
        }

        Image {
            fillMode: Image.PreserveAspectCrop

            anchors.centerIn: parent
            source: imageData.data.imageURL

            width: mediaView.cellWidth - 4
            height: mediaView.cellHeight - 4

            sourceSize.width: width
            sourceSize.height: height
        }
    }
}
