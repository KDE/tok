import QtQuick 2.10
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.12 as Kirigami

import "qrc:/components" as Components

Kirigami.PageRoute {

name: "Entry/Password"

Kirigami.Page {
    header: Components.Header {
        Kirigami.Heading {
            text: i18n("Password")

            Layout.margins: Kirigami.Units.largeSpacing
            Layout.fillWidth: true
        }
    }

    Kirigami.Theme.colorSet: Kirigami.Theme.View

    ColumnLayout {
        anchors {
            left: parent.left
            right: parent.right
        }

        Kirigami.Heading {
            text: i18n("You have enabled Two-Step Verification, so your account is protected with an additional password.")
            wrapMode: Text.Wrap
            level: 4
            opacity: 0.8

            Layout.fillWidth: true
            Layout.bottomMargin: Kirigami.Units.largeSpacing
        }

        RowLayout {
            layoutDirection: Qt.application.layoutDirection

            Kirigami.PasswordField {
                id: field
                onAccepted: tClient.enterPassword(text)

                Component.onCompleted: this.forceActiveFocus()
                Layout.fillWidth: true
            }
            QQC2.Button {
                icon.name: Qt.application.layoutDirection == Qt.LeftToRight ? "arrow-right" : "arrow-left"
                onClicked: field.accepted()

                Accessible.name: i18n("Continue")
                QQC2.ToolTip.text: i18n("Continue")
                QQC2.ToolTip.visible: hovered
            }

            Layout.fillWidth: true
        }
    }
}

}
