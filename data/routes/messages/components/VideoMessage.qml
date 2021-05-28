import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtGraphicalEffects 1.15
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.12 as Kirigami
import org.kde.Tok 1.0 as Tok

import "qrc:/components" as Components

Image {
    id: image

    source: videoData.data.videoThumbnail

    readonly property real ratio: width / implicitWidth

    Accessible.name: i18n("Photo message.")

    smooth: true
    mipmap: true

    HoverHandler {
        cursorShape: Qt.PointingHandCursor
    }
    TapHandler {
        onTapped: tClient.messagesStore.openVideo(del.mChatID, del.mID)
    }

    Image {
        id: blurImage

        source: videoData.data.videoThumbnail
        anchors.fill: parent

        visible: false
    }

    FastBlur {
        anchors.fill: blurImage
        source: blurImage
        radius: 32

        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Item {
                width: blurImage.width
                height: blurImage.height

                Rectangle {
                    width: scrim.width
                    height: scrim.height
                    radius: scrim.radius

                    anchors.centerIn: parent
                }
                Rectangle {
                    width: timestamp.width
                    height: timestamp.height
                    x: timestamp.x
                    y: timestamp.y

                    radius: 3
                }
            }
        }
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
        }
    }

    QQC2.Label {
        id: timestamp

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
    Layout.maximumWidth: settings.thinMode ? -1 : del.recommendedSize
    Layout.leftMargin: Kirigami.Units.largeSpacing
}
