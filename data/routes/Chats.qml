import QtQuick 2.10
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

Kirigami.PageRoute {

name: "Chats"

Kirigami.ScrollablePage {
    ListView {
        model: tClient.chatsModel

        delegate: Kirigami.BasicListItem {
            id: del

            required property string mTitle
            required property string mLastMessageAuthorID
            required property string mLastMessageContent
            required property string mPhoto
            required property string mID

            text: mTitle
            subtitle: {
                const content = mLastMessageContent.split('\n')[0]

                if (mLastMessageAuthorID !== "") {
                    return `${userData.name}: ${content}`
                }

                return content
            }

            Tok.UserData {
                id: userData

                userID: del.mLastMessageAuthorID
                client: tClient
            }

            leading: Kirigami.Avatar {
                name: del.mTitle

                width: height
            }

            onClicked: {
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
