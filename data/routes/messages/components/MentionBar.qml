import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

ListView {
    id: autoCompleteThing
    property string filter
    onFilterChanged: {
        if (autoCompleteThing.count > 0 && autoCompleteThing.currentIndex == -1) {
            autoCompleteThing.currentIndex = 0
        }
    }

    function up(event) {
        if (autoCompleteThing.count == 0 || !autoCompleteThing.visible) {
            event.accepted = false
            return
        }
        autoCompleteThing.incrementCurrentIndex()
    }

    function down(event) {
        if (autoCompleteThing.count == 0 || !autoCompleteThing.visible) {
            event.accepted = false
            return
        }
        autoCompleteThing.decrementCurrentIndex()
    }

    function tab(event) {
        if (autoCompleteThing.count == 0 || !autoCompleteThing.visible) {
            event.accepted = false
            return false
        }

        txtField.insert(txtField.cursorPosition, autoCompleteThing.currentItem.data.username.slice(autoCompleteThing.filter.length) + " ")
        return true
    }

    model: Tok.UserSortModel {
        sourceModel: tClient.membersModel(chatData.data.mKindID, chatData.data.mKind)
        store: tClient.userDataModel
        filter: autoCompleteThing.filter
    }
    reuseItems: true
    verticalLayoutDirection: ListView.BottomToTop
    delegate: Kirigami.BasicListItem {
        id: del

        required property string userID
        required property int index
        property alias data: userData.data

        text: userData.data.name + (userData.data.username == "" ? "" : ` - @${userData.data.username}`)
        reserveSpaceForSubtitle: true

        onClicked: {
            autoCompleteThing.currentIndex = del.index
            autoCompleteThing.tab(null)
        }

        leading: Kirigami.Avatar {
            width: height
            source: userData.data.smallAvatar
            name: userData.data.name
        }

        background: Rectangle {
            Kirigami.Theme.colorSet: del.ListView.isCurrentItem ? Kirigami.Theme.Selection : Kirigami.Theme.Window
            Kirigami.Theme.inherit: false

            color: Kirigami.Theme.backgroundColor
        }

        Tok.RelationalListener {
            id: userData

            model: tClient.userDataModel
            key: del.userID
            shape: QtObject {
                required property string name
                required property string username
                required property string smallAvatar
            }
        }
    }
}