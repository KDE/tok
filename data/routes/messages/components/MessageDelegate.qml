import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

import "qrc:/components" as GlobalComponents

QQC2.Control {
    id: del

    required property string mID
    required property string mChatID
    required property string mNextID
    required property string mPreviousID

    readonly property int recommendedSize: (applicationWindow().wideScreen ? Math.max(del.width / 3, Kirigami.Units.gridUnit * 15) : (del.width * 0.8))

    readonly property bool isOwnMessage: messageData.data.authorID === tClient.ownID
    readonly property bool showAvatar: (nextData.data.authorID != messageData.data.authorID) && (!(Kirigami.Settings.isMobile && isOwnMessage))
    readonly property bool separateFromPrevious: previousData.data.authorID != messageData.data.authorID

    topPadding: del.separateFromPrevious ? Kirigami.Units.largeSpacing : Kirigami.Units.smallSpacing
    bottomPadding: 0

    Accessible.role: Accessible.ListItem
    Accessible.name: tryit(() => __loaderSwitch.item.Accessible.name)

    Kirigami.Theme.colorSet: {
        return Kirigami.Theme.Button
        // if (Array.from(messagesSelectionModel.selectedIndexes).includes(modelIndex)) {
        //     return Kirigami.Theme.Selection
        // }
        // return messagesRoute.model.userID() == authorID ? Kirigami.Theme.Button : Kirigami.Theme.Window
    }
    Kirigami.Theme.inherit: false

    contentItem: RowLayout {
        Kirigami.Avatar {
            name: userData.data.name
            source: userData.data.smallAvatar

            implicitWidth: Kirigami.Units.gridUnit*2
            implicitHeight: Kirigami.Units.gridUnit*2

            // visible yoinks from layout, which isn't what we want.
            opacity: del.showAvatar ? 1 : 0

            HoverHandler {
                cursorShape: Qt.PointingHandCursor
            }
            TapHandler {
                onTapped: __userDataSheet.open()
            }

            GlobalComponents.UserDataSheet { id: __userDataSheet; userID: messageData.data.authorID || "" }

            Layout.alignment: Qt.AlignBottom
        }

        Item {
            Layout.fillWidth: (del.isOwnMessage && Kirigami.Settings.isMobile)
        }

        GlobalComponents.LoaderSwitch {
            id: __loaderSwitch

            value: messageData.data.kind
            cases: {
                "messageText": Qt.resolvedUrl("TextMessage.qml"),
                "messagePhoto": Qt.resolvedUrl("PhotoMessage.qml"),
            }
            defaultCase: Qt.resolvedUrl("Unsupported.qml")
        }

        Item {
            Layout.fillWidth: !(del.isOwnMessage && Kirigami.Settings.isMobile)
        }
    }

    width: parent && parent.width > 0 ? parent.width : implicitWidth
    Layout.fillWidth: true

    Tok.RelationalListener {
        id: messageData

        model: tClient.messagesStore
        key: [del.mChatID, del.mID]
        shape: QtObject {
            required property string authorID
            required property string kind
            required property string timestamp
        }
    }
    Tok.RelationalListener {
        id: previousData

        model: tClient.messagesStore
        key: [del.mChatID, del.mPreviousID]
        shape: QtObject {
            required property string authorID
        }
    }
    Tok.RelationalListener {
        id: nextData

        model: tClient.messagesStore
        key: [del.mChatID, del.mNextID]
        shape: QtObject {
            required property string authorID
        }
    }
    Tok.RelationalListener {
        id: userData

        model: tClient.userDataModel
        key: messageData.data.authorID
        shape: QtObject {
            required property string name
            required property string smallAvatar
        }
    }
}