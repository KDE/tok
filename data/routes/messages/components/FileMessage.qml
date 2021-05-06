import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

QQC2.Control {
    id: fileMessageRoot

    topPadding: Kirigami.Units.largeSpacing
    bottomPadding: Kirigami.Units.largeSpacing
    leftPadding: Kirigami.Units.largeSpacing+tailSize
    rightPadding: Kirigami.Units.largeSpacing

    readonly property int tailSize: Kirigami.Units.largeSpacing

    Accessible.name: `${userData.data.name} uploaded a file: ${fileData.data.fileName}`

    Tok.RelationalListener {
        id: fileData

        model: tClient.messagesStore
        key: [del.mChatID, del.mID]
        shape: QtObject {
            required property string fileName
            required property string fileCaption
        }
    }

    background: Item {
        clip: true

        Rectangle {
            color: Kirigami.Theme.backgroundColor
            anchors.fill: tail
            anchors.topMargin: 4
            anchors.rightMargin: -fileMessageRoot.tailSize
            visible: del.showAvatar
        }
        Kirigami.ShadowedRectangle {
            id: tail

            visible: del.showAvatar
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                leftMargin: -fileMessageRoot.tailSize*2
            }
            width: fileMessageRoot.tailSize*3
            color: Kirigami.Theme.backgroundColor

            corners {
                topLeftRadius: 0
                topRightRadius: 0
                bottomRightRadius: fileMessageRoot.tailSize*10
                bottomLeftRadius: 0
            }

            Kirigami.Theme.colorSet: Kirigami.Theme.View
            Kirigami.Theme.inherit: false
        }
        Kirigami.ShadowedRectangle {
            id: mainBG
            corners {
                topLeftRadius: 4
                topRightRadius: 4
                bottomRightRadius: 4
                bottomLeftRadius: 4
            }
            color: Kirigami.Theme.backgroundColor
            anchors.fill: parent
            anchors.leftMargin: fileMessageRoot.tailSize
        }
        QQC2.Label {
            id: timestamp
            text: messageData.data.timestamp
            opacity: 0.5

            font.pointSize: -1
            font.pixelSize: Kirigami.Units.gridUnit * (2/3)
            anchors {
                bottom: parent.bottom
                right: mainBG.right
                margins: Kirigami.Units.smallSpacing
            }
            LayoutMirroring.enabled: Tok.Utils.isRTL(fileData.data.fileName)
        }
        QQC2.Label {
            id: dummy
            text: " "
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
        ReplyBlock {}
        QQC2.Label {
            text: fileData.data.fileName + (fileData.data.fileCaption == "" ? paddingT : "")
            wrapMode: Text.Wrap

            readonly property string paddingT: " ".repeat(Math.ceil(timestamp.implicitWidth / dummy.implicitWidth)) + "⠀"

            Layout.fillWidth: true
        }
        QQC2.Label {
            text: fileData.data.fileCaption + paddingT
            wrapMode: Text.Wrap
            visible: fileData.data.fileCaption != ""

            readonly property string paddingT: " ".repeat(Math.ceil(timestamp.implicitWidth / dummy.implicitWidth)) + "⠀"

            Layout.fillWidth: true
        }
    }

    Layout.maximumWidth: del.recommendedSize
}