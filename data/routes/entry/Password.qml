import QtQuick 2.10
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.12 as Kirigami

Kirigami.PageRoute {

name: "Entry/Password"

Kirigami.ScrollablePage {
    ColumnLayout {
        Kirigami.FormLayout {
            Kirigami.Heading {
                text: i18n("Enter your password")
            }

            Kirigami.PasswordField {
                Component.onCompleted: this.forceActiveFocus()
                onAccepted: tClient.enterPassword(text)
            }
        }

    }
}

}
