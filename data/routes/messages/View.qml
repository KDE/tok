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

    footer: QQC2.ToolBar {
        RowLayout {
            id: composeRow

            function send() {
                lView.model.send(txtField.text)
                txtField.text = ""
            }

            QQC2.TextField {
                id: txtField

                background: null
                onAccepted: composeRow.send()

                Layout.fillWidth: true
            }
            QQC2.Button {
                icon.name: "document-send"
                onClicked: composeRow.send()
            }

            anchors {
                left: parent.left
                right: parent.right
            }
        }
    }

    ListView {
        id: lView

        reuseItems: true
        verticalLayoutDirection: ListView.BottomToTop

        delegate: Components.MessageDelegate {}
    }
}

}
