import QtQuick 2.10
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami

Kirigami.PageRoute {

name: "Chats"

Kirigami.ScrollablePage {
    ListView {
        model: tClient.chatsModel

        delegate: Kirigami.BasicListItem {
            id: del

            required property string mTitle
            required property string mSubtitle
            required property string mPhoto

            text: mTitle
            subtitle: mSubtitle

            leading: Kirigami.Avatar {
                name: del.mTitle

                width: height
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
