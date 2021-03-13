import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

import "components" as Components

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

        reuseItems: true
        verticalLayoutDirection: ListView.BottomToTop

        delegate: Components.MessageDelegate {}
    }
}

}
