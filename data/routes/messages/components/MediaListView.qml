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

required property string title
required property var model
required property Component delegate
property bool isWide: false

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
                onClicked: root.parent.pop()
                visible: root.parent instanceof QQC2.StackView
            }
            Kirigami.Heading {
                level: 4
                text: root.title

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
            id: cellView

            model: root.model
            delegate: root.delegate
        }

        Layout.fillHeight: true
        Layout.fillWidth: true
    }
}

}