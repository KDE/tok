import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.12 as Kirigami

Kirigami.PageRoute {

name: "Entry/Welcome"

Kirigami.Page {
    ColumnLayout {
        Kirigami.Heading {
            text: i18n("Welcome to Tok")
        }

        QQC2.Button {
            text: i18n("Get Started")

            onClicked: Kirigami.PageRouter.navigateToRoute("Entry/PhoneNumber")
        }
    }
}

}
