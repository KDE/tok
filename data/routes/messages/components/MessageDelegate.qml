import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

import "qrc:/components" as GlobalComponents

QQC2.Control {
    id: del

    required property string mContent
    required property string mAuthorID
    required property string mPreviousAuthorID
    required property string mNextAuthorID
    required property string mID
    required property string mKind

    readonly property bool isOwnMessage: mAuthorID === tClient.ownID
    readonly property bool showAvatar: (mNextAuthorID != mAuthorID) && (!(Kirigami.Settings.isMobile && isOwnMessage))
    readonly property bool separateFromPrevious: mPreviousAuthorID != mAuthorID

    topPadding: del.separateFromPrevious ? Kirigami.Units.largeSpacing : Kirigami.Units.smallSpacing
    bottomPadding: 0

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
            name: userData.name
            source: userData.smallAvatar

            implicitWidth: Kirigami.Units.gridUnit*2
            implicitHeight: Kirigami.Units.gridUnit*2

            // visible yoinks from layout, which isn't what we want.
            opacity: del.showAvatar ? 1 : 0

            Layout.alignment: Qt.AlignBottom
        }

        Item {
            Layout.fillWidth: (del.isOwnMessage && Kirigami.Settings.isMobile)
        }

        GlobalComponents.LoaderSwitch {
            value: del.mKind
            cases: {
                "messageText": Qt.resolvedUrl("TextMessage.qml")
            }
            defaultCase: Qt.resolvedUrl("Unsupported.qml")
        }

        Item {
            Layout.fillWidth: !(del.isOwnMessage && Kirigami.Settings.isMobile)
        }
    }

    width: parent && parent.width > 0 ? parent.width : implicitWidth
    Layout.fillWidth: true

    Tok.UserData {
        id: userData

        userID: del.mAuthorID
        client: tClient
    }
}