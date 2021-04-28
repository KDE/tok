import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

import "components" as Components
import "qrc:/components" as GlobalComponents

Kirigami.PageRoute {

name: "Messages/View"

Kirigami.ScrollablePage {
    id: pageRoot

    property string chatID
    onChatIDChanged: {
        lView.model = tClient.messagesModel(pageRoot.chatID)
    }

    Tok.RelationalListener {
        id: chatData

        model: tClient.chatsStore
        key: pageRoot.chatID
        shape: QtObject {
            required property string mTitle
            required property bool mCanSendMessages
        }
    }

    header: GlobalComponents.Header {
        RowLayout {
            QQC2.ToolButton {
                icon.name: "arrow-left"
                onClicked: Kirigami.PageRouter.bringToView(0)
                visible: !applicationWindow().wideScreen
            }
            Kirigami.Heading {
                level: 4
                text: chatData.data.mTitle

                verticalAlignment: Text.AlignVCenter
                Layout.fillHeight: true
                Layout.margins: Kirigami.Units.largeSpacing
            }
        }
    }

    footer: QQC2.ToolBar {
        RowLayout {
            id: composeRow

            function send() {
                lView.model.send(txtField.text)
                txtField.text = ""
            }

            QQC2.TextArea {
                id: txtField

                background: null
                enabled: chatData.data.mCanSendMessages

                placeholderText: enabled ? i18n("Write your message...") : i18n("You cannot send messages.")

                Keys.onReturnPressed: (event) => {
                    if (!(event.modifiers & Qt.ShiftModifier)) {
                        composeRow.send()
                        event.accepted = true
                    } else {
                        event.accepted = false
                    }
                }
                Keys.onTabPressed: nextItemInFocusChain().forceActiveFocus(Qt.TabFocusReason)
                Layout.fillWidth: true
            }
            QQC2.Button {
                Accessible.name: i18n("Send message")
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
        activeFocusOnTab: true

        property var visibleItems: []

        onVisibleItemsChanged: {
            lView.model.messagesInView(visibleItems)
        }

        delegate: Components.MessageDelegate {
            function add() {
                lView.visibleItems = [...lView.visibleItems, this.mID]
            }
            function remove() {
                lView.visibleItems = lView.visibleItems.filter(it => it != this.mID)
            }

            Component.onCompleted: add()
            ListView.onReused: add()

            Component.onDestruction: remove()
            ListView.onPooled: remove()
        }
    }
}

}
