import QtQuick 2.10
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.12 as Kirigami

Kirigami.PageRoute {

name: "Entry/AuthenticationCode"

Kirigami.ScrollablePage {
    ColumnLayout {
        Kirigami.FormLayout {
            Kirigami.Heading {
                text: i18n("Enter your authentication code that you received from another Telegram client")
            }

            QQC2.TextField {
                Component.onCompleted: this.forceActiveFocus()
                placeholderText: i18n("Authentication code")
                onAccepted: tClient.enterCode(text)
            }
        }
    }
}

}
