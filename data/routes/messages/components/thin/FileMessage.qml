import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

QQC2.Control {
    id: fileMessageRoot

    topPadding: Kirigami.Units.smallSpacing
    bottomPadding: Kirigami.Units.smallSpacing
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
            text: fileData.data.fileName
            wrapMode: Text.Wrap

            Layout.fillWidth: true
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