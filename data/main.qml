import QtQuick 2.10
import QtQuick.Controls 2.10 as QQC2
import org.kde.kirigami 2.12 as Kirigami

Kirigami.ApplicationWindow {
    Connections {
        target: tClient
        function onPhoneNumberRequested() {
            console.log(`wants phone number`)
        }
        function onCodeRequested() {
            console.log(`wants code`)
        }
        function onPasswordRequested() {
            console.log(`wants password`)
        }
        function onLoggedIn() {
            console.log(`logged in`)
        }
        function onLoggedOut() {
            console.log(`logged out`)
        }
    }

    Kirigami.FormLayout {
        QQC2.TextField {
            onAccepted: tClient.enterPhoneNumber(text)
            Kirigami.FormData.label: "phone number"
        }
        QQC2.TextField {
            onAccepted: tClient.enterCode(text)
            Kirigami.FormData.label: "code"
        }
        QQC2.TextField {
            onAccepted: tClient.enterPassword(text)
            Kirigami.FormData.label: "password"
        }
    }
}
