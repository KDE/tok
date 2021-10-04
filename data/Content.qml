// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.10
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.10 as QQC2
import org.kde.kirigami 2.12 as Kirigami
import org.kde.Tok 1.0 as Tok
import QtGraphicalEffects 1.15
import QtMultimedia 5.15

import Qt.labs.settings 1.0
import Qt.labs.platform 1.1

import org.kde.sonnet 1.0 as Sonnet

import "routes" as Routes
import "routes/entry" as EntryRoutes
import "routes/messages" as MessagesRoutes

import "qrc:/components" as Components
import "qrc:/routes/settings" as SettingsComponents

import "qrc:/components/upload/desktop" as Desktop
import "qrc:/components/upload/mobile" as Mobile

Kirigami.PageRow {
    id: rootRow

    globalToolBar.style: Kirigami.ApplicationHeaderStyle.None

    property alias router: rootRouter
    property alias settingsWindow: settingsWindow

    property int rightOffset: 0
    property int leftOffset: 0

    property Binding binding: Binding {
        target: tClient
        property: "online"
        value: rootWindow.active
    }

    Loader {
        source: Qt.resolvedUrl("components/GlobalMenu.qml")
        asynchronous: true
    }

    function tryit(fn, def) {
        try {
            return fn() ?? def
        } catch (e) {
            return def
        }
    }

    function closing(event) {
        if (settings.userWantsSystemTray) {
            event.accepted = false
            rootWindow.hide()
            return
        }

        if (Components.AudioPlayer.playbackState == Audio.PlayingState) {
            event.accepted = false
            rootWindow.showMinimized()
            return
        }
    }

    MouseArea {
        visible: rootRow.shouldUseSidebars
        z: 500

        anchors.top: parent.top
        anchors.bottom: parent.bottom

        x: rootRow.pageWidth - (width / 2)
        width: Kirigami.Units.devicePixelRatio * 2

        property int _lastX: -1

        cursorShape: Qt.SplitHCursor

        onPressed: _lastX = mouseX

        onPositionChanged: {
            if (_lastX == -1) return

            if (mouse.x > _lastX) {
                settings.pageWidth = Math.min((rootRow.defaultPageWidth),
                    rootRow.pageWidth + (mouse.x - _lastX));
            } else if (mouse.x < _lastX) {
                settings.pageWidth = Math.max((rootRow.defaultPageWidth - rootRow.leeway),
                    rootRow.pageWidth - (_lastX - mouse.x));
            }
        }
    }

    Sonnet.SpellcheckHighlighter {
        id: theOneTrueSpellCheckHighlighter

        readonly property Item field: (rootWindow.activeFocusItem instanceof TextEdit || rootWindow.activeFocusItem instanceof TextInput) ? rootWindow.activeFocusItem : null
        property var suggestions_: []
        readonly property Connections conns: Connections {
            target: theOneTrueSpellCheckHighlighter.field
            function onCursorPositionChanged() {
                theOneTrueSpellCheckHighlighter.suggestions_ = theOneTrueSpellCheckHighlighter.suggestions(theOneTrueSpellCheckHighlighter.field.selectionStart)
            }
        }

        document: field.textDocument || null
        cursorPosition: field.cursorPosition
        selectionStart: field.selectionStart
        selectionEnd: field.selectionEnd
        misspelledColor: Kirigami.Theme.negativeTextColor
        active: field instanceof TextEdit && !field.readOnly

        onChangeCursorPosition: {
            field.cursorPosition = start
            field.moveCursorSelection(end, TextEdit.SelectCharacters)
        }
    }

    SystemTrayIcon {
        visible: settings.userWantsSystemTray
        icon.name: "org.kde.Tok"

        onActivated: {
            if (reason === SystemTrayIcon.Trigger) {
                rootWindow.show()
                rootWindow.requestActivate()
            }
        }

        menu: Menu {
            MenuItem {
                text: tClient.doNotDisturb ? i18nc("menu", "Enable Notifications") : i18nc("menu", "Disable Notifications")
                onTriggered: tClient.doNotDisturb = !tClient.doNotDisturb
            }
            MenuItem {
                text: i18nc("menu", "Quit")
                onTriggered: Qt.quit()
            }
        }
    }

    Loader {
        id: audioBar

        active: Components.AudioPlayer.status != Audio.NoMedia

        z: 300

        sourceComponent: QQC2.Control {
            parent: rootRow
            z: 300

            x: !rootRow.shouldUseSidebars ? 0 : rootRow.lastVisibleItem.x-rootRow.columnView.contentX
            width: rootRow.lastVisibleItem.width
            y: heady.height

            Components.Header { id: heady; width: 30; opacity: 0; y: -100 }

            background: Rectangle {
                id: reccy

                color: Kirigami.Theme.backgroundColor

                Rectangle {
                    width: (Components.AudioPlayer.position/Components.AudioPlayer.duration) * reccy.width
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                        left: parent.left
                    }
                    color: Kirigami.Theme.focusColor
                    opacity: 0.3
                }

                Kirigami.Separator {
                    anchors {
                        bottom: parent.bottom
                        left: parent.left
                        right: parent.right
                    }
                }
            }

            contentItem: RowLayout {
                QQC2.ToolButton {
                    icon.name: Components.AudioPlayer.playbackState == Audio.PlayingState ? "media-playback-pause" : "media-playback-start"
                    icon.width: Kirigami.Units.iconSizes.small
                    icon.height: Kirigami.Units.iconSizes.small
                    onClicked: Components.AudioPlayer.playbackState == Audio.PlayingState ? Components.AudioPlayer.pause() : Components.AudioPlayer.play()
                }
                QQC2.Label {
                    text: Components.AudioPlayer.metaData.title || Components.AudioPlayer.source
                    Layout.fillWidth: true
                }
                QQC2.ToolButton {
                    icon.name: "dialog-close"
                    icon.width: Kirigami.Units.iconSizes.small
                    icon.height: Kirigami.Units.iconSizes.small
                    onClicked: Components.AudioPlayer.clear()
                }
            }
        }
    }

    readonly property int defaultPageWidth: Kirigami.Units.gridUnit * 20
    readonly property int leeway: (Kirigami.Units.gridUnit * 5)
    readonly property alias pageWidth: settings.pageWidth
    defaultColumnWidth: pageWidth

    readonly property bool shouldUseSidebars: (rootWindow.width-300) >= (rootRow.defaultPageWidth*2)
    columnView.columnResizeMode: shouldUseSidebars && rootRow.depth > 1 ? Kirigami.ColumnView.FixedColumns : Kirigami.ColumnView.SingleColumn
    anchors.rightMargin: rootRouter.pageHasSidebar ? 300 : 0

    property Settings settings: Settings {
        id: settings

        property bool thinMode: false
        property bool imageBackground: true
        property bool transparent: false
        property bool userWantsSidebars: true
        property bool userHasDownloadedFile: false
        property bool userWantsSystemTray: false
        property int pageWidth: -1

        onTransparentChanged: {
            Tok.Utils.setBlur(rootRow, transparent)
        }
        Component.onCompleted: {
            if (settings.pageWidth == -1) {
                settings.pageWidth = Kirigami.Units.gridUnit * 20
            }
            Tok.Utils.setBlur(rootRow, transparent)
            rootRow.defaultColumnWidth = Qt.binding(() => settings.pageWidth)
        }
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

    SettingsComponents.Settings {
        id: settingsWindow
    }
    Desktop.UploadDialog {
        id: desktopPicker
    }
    Mobile.UploadDialog {
        id: mobilePicker
    }
    Components.UserDataSheet {
        id: globalUserDataSheet
        userID: ""
        chatID: ""
    }

    QQC2.Popup {
        id: deleteDialog

        modal: true
        parent: rootRow.QQC2.Overlay.overlay
        x: (QQC2.Overlay.overlay.width / 2) - (this.width / 2)
        y: (QQC2.Overlay.overlay.height / 2) - (this.height / 2)

        property string title
        property string chatID
        property string messageID
        property bool canDeleteForSelf: false
        property bool canDeleteForOthers: false

        padding: Kirigami.Units.gridUnit

        contentItem: ColumnLayout {
            Kirigami.Heading {
                text: i18n("Do you want to delete this message?")
                level: 4

                Layout.bottomMargin: Kirigami.Units.gridUnit
            }
            QQC2.Label {
                visible: deleteDialog.canDeleteForSelf && !deleteDialog.canDeleteForOthers
                text: i18n("This will delete it just for you.")
            }
            QQC2.Label {
                visible: !deleteDialog.canDeleteForSelf && deleteDialog.canDeleteForOthers
                text: i18n("This will delete it for everyone in this chat.")
            }
            QQC2.CheckBox {
                id: deleteForAll

                checked: true
                text: deleteDialog.chatID[0] == "-" ? i18n("Also delete for everyone") : i18n("Also delete for %1", deleteDialog.title)
                visible: deleteDialog.canDeleteForSelf && deleteDialog.canDeleteForOthers
            }
            // QQC2.CheckBox {
            //     text: i18n("Report Spam")
            // }
            // QQC2.CheckBox {
            //     text: i18n("Delete all from this user")
            // }
            RowLayout {
                Layout.topMargin: Kirigami.Units.gridUnit

                Item { Layout.fillWidth: true }
                QQC2.Button {
                    text: i18n("Cancel")
                    onClicked: deleteDialog.close()
                }
                QQC2.Button {
                    text: i18n("Delete")
                    onClicked: {
                        tClient.messagesStore.deleteMessage(deleteDialog.chatID, deleteDialog.messageID, deleteForAll.checked)
                        deleteDialog.close()
                    }
                }
            }
        }
    }

    Kirigami.PageRouter {
        id: rootRouter
        pageStack: rootRow.columnView
        initialRoute: ""

        property bool pageHasSidebar: rootRow.shouldUseSidebars && settings.userWantsSidebars && pageCanHaveSidebar
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

    Kirigami.Heading {
        level: 4

        visible: tClient.connectionState != Tok.Client.Ready

        text: {
            switch (tClient.connectionState) {
            case Tok.Client.Connecting:
                return i18nc("small bubble in bottom left of window indicating current connection status", "Connecting…")
            case Tok.Client.ConnectingToProxy:
                return i18nc("small bubble in bottom left of window indicating current connection status", "Connecting to proxy…")
            case Tok.Client.WaitingForNetwork:
                return i18nc("small bubble in bottom left of window indicating current connection status", "Waiting on network…")
            case Tok.Client.Updating:
                return i18nc("small bubble in bottom left of window indicating current connection status", "Updating…")
            }
        }

        padding: Kirigami.Units.smallSpacing
        leftPadding: Kirigami.Units.largeSpacing
        rightPadding: Kirigami.Units.largeSpacing

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 6

        z: 99

        background: Rectangle {
            radius: height

            Kirigami.Theme.colorSet: Kirigami.Theme.Window
            color: Kirigami.Theme.backgroundColor

            layer.enabled: true
            layer.effect: DropShadow {
                cached: true
                horizontalOffset: 0
                verticalOffset: 1
                radius: 2.0
                samples: 17
                color: "#30000000"
            }
        }
    }
}
