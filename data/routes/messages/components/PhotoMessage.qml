import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtGraphicalEffects 1.15
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.12 as Kirigami
import org.kde.Tok 1.0 as Tok

import "qrc:/components" as Components

Image {
    id: image

    source: imageData.data.imageURL

    readonly property real ratio: width / implicitWidth

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
        source: image.source
    }

    Tok.RelationalListener {
        id: imageData

        model: tClient.messagesStore
        key: [del.mChatID, del.mID]
        shape: QtObject {
            required property string imageURL
            required property string imageCaption
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

    Layout.preferredHeight: implicitHeight * ratio
    Layout.maximumWidth: del.recommendedSize
    Layout.leftMargin: Kirigami.Units.largeSpacing
}
