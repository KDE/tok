import QtQuick 2.10
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.kitemmodels 1.0
import org.kde.Tok 1.0 as Tok
import "qrc:/components" as Components

Kirigami.PageRoute {

name: "Chats"

Kirigami.ScrollablePage {
    header: Components.Header {
        RowLayout {
            Layout.margins: Kirigami.Units.largeSpacing
            Layout.fillWidth: true

            Kirigami.SearchField {
                id: searchField

                Layout.fillWidth: true
            }
        }
    }

    ListView {
        model: Tok.ChatSortModel {
            sourceModel: tClient.chatsModel
        }
        activeFocusOnTab: true

        delegate: Kirigami.BasicListItem {
            id: del

            required property string mID
            required property string mMainListPosition

            Tok.RelationalListener {
                id: chatData

                model: tClient.chatsStore
                key: del.mID
                shape: QtObject {
                    required property string mTitle
                    required property string mLastMessageID
                    required property string mPhoto
                    required property int    mUnreadCount
                }
            }

            topPadding: Kirigami.Units.largeSpacing
            bottomPadding: Kirigami.Units.largeSpacing

            text: chatData.data.mTitle + ` ${del.mMainListPosition}`
            subtitle: `${plaintext.hasAuthor ? plaintext.authorName + ": " : ""}${plaintext.onelinePlaintext}`

            Components.PlaintextMessage {
                id: plaintext

                messagesModel: tClient.messagesStore
                userModel: tClient.userDataModel

                chatID: del.mID
                messageID: chatData.data.mLastMessageID
            }

            checked: Kirigami.PageRouter.router.params.chatID === del.mID
            checkable: Kirigami.PageRouter.router.params.chatID === del.mID
            highlighted: false

            leading: Kirigami.Avatar {
                name: chatData.data.mTitle
                source: chatData.data.mPhoto

                width: height
            }
            trailing: RowLayout {
                QQC2.Label {
                    text: chatData.data.mUnreadCount
                    visible: chatData.data.mUnreadCount > 0
                    padding: Kirigami.Units.smallSpacing

                    horizontalAlignment: Qt.AlignHCenter

                    Layout.preferredWidth: implicitHeight

                    background: Rectangle {
                        color: Kirigami.Theme.focusColor
                        radius: height / 2
                    }
                }
            }

            onClicked: {
                if (Kirigami.PageRouter.router.params.chatID !== undefined) {
                    tClient.messagesModel(Kirigami.PageRouter.router.params.chatID).comingOut()
                }
                tClient.messagesModel(del.mID).comingIn()
                Kirigami.PageRouter.pushFromHere({ "route": "Messages/View", "chatID": del.mID })
            }
        }

        Kirigami.PlaceholderMessage {
            text: i18n("No chats available.")

            anchors.centerIn: parent

            visible: parent.count === 0
        }
    }
}

}
