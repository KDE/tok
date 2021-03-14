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
            required property string mLastMessageAuthorID
            required property string mLastMessageContent
            required property string mPhoto
            required property string mID
            required property int    mUnreadCount

            topPadding: Kirigami.Units.largeSpacing
            bottomPadding: Kirigami.Units.largeSpacing

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
