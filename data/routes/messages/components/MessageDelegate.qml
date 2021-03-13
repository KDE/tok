import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

QQC2.Control {
    id: del

    required property string mContent
    required property string mAuthorID
    required property string mPreviousAuthorID
    required property string mNextAuthorID

    readonly property bool showAvatar: mNextAuthorID != mAuthorID
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
        QQC2.Control {
            topPadding: Kirigami.Units.largeSpacing
            bottomPadding: Kirigami.Units.largeSpacing
            leftPadding: Kirigami.Units.largeSpacing
            rightPadding: Kirigami.Units.largeSpacing

            background: Rectangle {
                radius: 4
                color: Kirigami.Theme.backgroundColor
            }
            contentItem: ColumnLayout {
                QQC2.Label {
                    text: userData.name
                    color: Kirigami.NameUtils.colorsFromString(text)

                    visible: del.separateFromPrevious

                    wrapMode: Text.Wrap

                    Layout.fillWidth: true
                }
                TextEdit {
                    text: del.mContent

                    readOnly: true
                    selectByMouse: true
                    wrapMode: Text.Wrap

                    color: Kirigami.Theme.textColor
                    selectedTextColor: Kirigami.Theme.highlightedTextColor
                    selectionColor: Kirigami.Theme.highlightColor

                    Layout.fillWidth: true
                }
            }

            Layout.maximumWidth: (applicationWindow().wideScreen ? Math.max(del.width / 3, Kirigami.Units.gridUnit * 15) : (del.width * 0.9))
        }
        Item {
            Layout.fillWidth: true
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