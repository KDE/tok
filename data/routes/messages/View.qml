import QtQuick 2.10
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami

Kirigami.PageRoute {

name: "Messages/View"

Kirigami.ScrollablePage {
    id: pageRoot

    property string chatID
    onChatIDChanged: {
        lView.model = tClient.messagesModel(pageRoot.chatID)
    }

    ListView {
        id: lView

        delegate: Kirigami.BasicListItem {
            id: del

            required property string mContent

            text: mContent
        }
    }
}

}
