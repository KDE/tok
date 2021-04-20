import QtQuick 2.10
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.12 as Kirigami

Kirigami.PageRoute {

name: "Entry/PhoneNumber"

Kirigami.ScrollablePage {
    ColumnLayout {
        Kirigami.FormLayout {
            Kirigami.Heading {
                text: i18n("Enter your phone number")
            }

            QQC2.TextField {
                Component.onCompleted: this.forceActiveFocus()
                placeholderText: i18n("Phone number")
                onAccepted: tClient.enterPhoneNumber(text)
            }
        }

    }
}

}
