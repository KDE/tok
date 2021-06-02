import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

import QtQuick.Dialogs 1.0 as Dialogues

import "qrc:/components" as GlobalComponents

QQC2.Control {

id: root

padding: 0
topPadding: 0
leftPadding: 0
rightPadding: 0
bottomPadding: 0

Kirigami.Theme.inherit: false
Kirigami.Theme.colorSet: Kirigami.Theme.View

background: Rectangle {
    color: Kirigami.Theme.backgroundColor
}

contentItem: ColumnLayout {
    GlobalComponents.Header {
        RowLayout {
            QQC2.ToolButton {
                icon.name: "arrow-left"
                onClicked: rootRow.layers.pop()
                visible: root.parent instanceof QQC2.StackView
            }
            Kirigami.Heading {
                level: 4
                text: chatData.key[0] == "-" ? i18n("Group Info") : i18n("Info")

                verticalAlignment: Text.AlignVCenter
                Layout.fillHeight: true
                Layout.margins: Kirigami.Units.largeSpacing
            }
        }
        Layout.fillWidth: true
    }
    QQC2.ScrollView {
        QQC2.ScrollBar.horizontal.policy: QQC2.ScrollBar.AlwaysOff

        ListView {
            model: tClient.membersModel(chatData.data.mKindID, chatData.data.mKind)
            header: ColumnLayout {
                Kirigami.Avatar {
                    source: chatData.data.mPhoto
                    name: chatData.data.mTitle

                    Layout.preferredWidth: Kirigami.Units.gridUnit * 5
                    Layout.preferredHeight: Kirigami.Units.gridUnit * 5
                    Layout.topMargin: Kirigami.Units.smallSpacing
                    Layout.alignment: Qt.AlignHCenter
                }
                Kirigami.Heading {
                    text: chatData.data.mTitle
                    level: 2

                    horizontalAlignment: Text.AlignHCenter
                    Layout.bottomMargin: Kirigami.Units.largeSpacing
                    Layout.fillWidth: true
                }

                width: root.width
            }
            delegate: Kirigami.BasicListItem {
                id: del

                required property string userID

                leading: Kirigami.Avatar {
                    name: userData.data.name
                    source: userData.data.smallAvatar

                    width: height
                }

                topPadding: Kirigami.Units.largeSpacing
                bottomPadding: Kirigami.Units.largeSpacing

                text: userData.data.name
                reserveSpaceForSubtitle: true

                Tok.RelationalListener {
                    id: userData

                    model: tClient.userDataModel
                    key: del.userID
                    shape: QtObject {
                        required property string name
                        required property string smallAvatar
                    }
                }
            }
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        Layout.fillHeight: true
        Layout.fillWidth: true
    }
}

}