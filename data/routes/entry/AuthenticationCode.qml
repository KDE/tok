import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.12 as Kirigami

Kirigami.PageRoute {

name: "Entry/AuthenticationCode"

Kirigami.Page {
    ColumnLayout {
        Kirigami.Heading {
            text: i18n("Enter your authentication code that you received from another Telegram client")
        }

        QQC2.TextField {
            onAccepted: tClient.enterCode(text)
        }

    }
}

}
