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
        model: KSortFilterProxyModel {
            sourceModel: tClient.chatsModel
            filterString: searchField.text
            filterRole: "mTitle"
        }

        delegate: Kirigami.BasicListItem {
            id: del

            required property string mTitle
            required property string mLastMessageID
            required property string mPhoto
            required property string mID
            required property int    mUnreadCount

            topPadding: Kirigami.Units.largeSpacing
            bottomPadding: Kirigami.Units.largeSpacing

            text: mTitle
            subtitle: `${plaintext.hasAuthor ? plaintext.authorName + ": " : ""}${plaintext.onelinePlaintext}`

            Components.PlaintextMessage {
                id: plaintext

                messagesModel: tClient.messagesStore
                userModel: tClient.userDataModel

                chatID: del.mID
                messageID: del.mLastMessageID
            }

            checked: Kirigami.PageRouter.router.params.chatID === del.mID
            checkable: Kirigami.PageRouter.router.params.chatID === del.mID
            highlighted: false

            leading: Kirigami.Avatar {
                name: del.mTitle
                source: del.mPhoto

                width: height
            }
            trailing: RowLayout {
                QQC2.Label {
                    text: del.mUnreadCount
                    visible: del.mUnreadCount > 0
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
