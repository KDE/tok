import QtQuick 2.10
import QtQuick.Controls 2.10 as QQC2
import org.kde.kirigami 2.12 as Kirigami

import "routes" as Routes
import "routes/entry" as EntryRoutes
import "routes/messages" as MessagesRoutes

Kirigami.RouterWindow {
    id: rootWindow

    initialRoute: ""

    pageStack.globalToolBar.style: Kirigami.ApplicationHeaderStyle.None

    property Connections conns: Connections {
        target: tClient
        function onPhoneNumberRequested() {
            rootWindow.router.navigateToRoute("Entry/PhoneNumber")
        }
        function onCodeRequested() {
            rootWindow.router.navigateToRoute("Entry/AuthenticationCode")
        }
        function onPasswordRequested() {
            rootWindow.router.navigateToRoute("Entry/Password")
        }
        function onLoggedIn() {
            console.log(rootWindow.wideScreen)
            if (rootWindow.wideScreen) {
                rootWindow.router.navigateToRoute(["Chats", "Messages/NoView"])
            } else {
                rootWindow.router.navigateToRoute("Chats")
            }
        }
        function onLoggedOut() {
            rootWindow.router.navigateToRoute("Entry/Welcome")
        }
    }

    Kirigami.PageRoute {
        name: ""
        Kirigami.Page {}
    }

    Routes.Chats {}

    EntryRoutes.AuthenticationCode {}
    EntryRoutes.Password {}
    EntryRoutes.PhoneNumber {}
    EntryRoutes.Welcome {}

    MessagesRoutes.View {}
    MessagesRoutes.NoView {}

}
