import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

Kirigami.Heading {
    id: header

    level: 4

    readonly property bool isOwn: addData.data.addedMembers.length == 1 && addData.data.addedMembers[0] == messageData.data.authorID
    text: {
        if (isOwn) {
            return i18n("%1 joined the group", userData.data.name)
        }

        return "Unsupported"
    }

    padding: Kirigami.Units.smallSpacing
    leftPadding: Kirigami.Units.largeSpacing
    rightPadding: Kirigami.Units.largeSpacing

    anchors.centerIn: parent

    Tok.RelationalListener {
        id: addData

        model: tClient.messagesStore
        key: [del.mChatID, del.mID]
        shape: QtObject {
            required property var addedMembers
        }
    }
}
