import QtQuick 2.10
import QtQuick.Controls 2.10 as QQC2
import org.kde.kirigami 2.12 as Kirigami
import org.kde.Tok 1.0 as Tok

import Qt.labs.settings 1.0

import "routes" as Routes
import "routes/entry" as EntryRoutes
import "routes/messages" as MessagesRoutes

Kirigami.PageRow {
    id: rootRow

    globalToolBar.style: Kirigami.ApplicationHeaderStyle.None

    property alias router: rootRouter

    property Binding binding: Binding {
        target: tClient
        property: "online"
        value: rootWindow.active
    }

    function tryit(fn, def) {
        try {
            return fn() ?? def
        } catch (e) {
            return def
        }
    }

    readonly property bool shouldUseSidebars: (rootWindow.width-300) >= (rootRow.defaultColumnWidth*2)
    columnView.columnResizeMode: shouldUseSidebars ? Kirigami.ColumnView.FixedColumns : Kirigami.ColumnView.SingleColumn
    anchors.rightMargin: rootRouter.pageHasSidebar ? 300 : 0

    property Settings settings: Settings {
        property bool thinMode: false
        property bool imageBackground: true
        property bool transparent: false

        onTransparentChanged: {
            Tok.Utils.setBlur(rootRow, transparent)
        }
        Component.onCompleted: Tok.Utils.setBlur(rootRow, transparent)
    }

    property Rectangle focusRect: Rectangle {
        parent: rootWindow.contentItem
        visible: rootWindow.activeFocusItem !== null && Boolean(rootWindow.activeFocusItem.showFocusRing)

        x: tryit(() => rootWindow.activeFocusItem.Kirigami.ScenePosition.x, 0)
        y: tryit(() => rootWindow.activeFocusItem.Kirigami.ScenePosition.y, 0)
        width: tryit(() => rootWindow.activeFocusItem.width, 0)
        height: tryit(() => rootWindow.activeFocusItem.height, 0)
        radius: tryit(() => rootWindow.activeFocusItem.radius, 3)

        color: "transparent"
        border {
            width: 3
            color: Kirigami.Theme.focusColor
        }
    }

    property Connections conns: Connections {
        target: tClient
        function onPhoneNumberRequested() {
            rootRouter.navigateToRoute("Entry/PhoneNumber")
        }
        function onCodeRequested() {
            rootRouter.navigateToRoute("Entry/AuthenticationCode")
        }
        function onPasswordRequested() {
            rootRouter.navigateToRoute("Entry/Password")
        }
        function onLoggedIn() {
            if (!Kirigami.Settings.isMobile) {
                rootRouter.navigateToRoute(["Chats", "Messages/NoView"])
            } else {
                rootRouter.navigateToRoute("Chats")
            }
        }
        function onLoggedOut() {
            rootRouter.navigateToRoute("Entry/Welcome")
        }
    }

    Kirigami.PageRouter {
        id: rootRouter
        pageStack: rootRow.columnView
        initialRoute: ""

        property bool pageHasSidebar: rootRow.shouldUseSidebars && pageCanHaveSidebar
        property bool pageCanHaveSidebar: false

        function evaluatePageHasSidebar() {
            const routes = [
                ["Chats", "Messages/View"]
            ]
            pageCanHaveSidebar = routes.some((it) => rootRouter.routeActive(it))
        }

        onNavigationChanged: evaluatePageHasSidebar()

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
}
