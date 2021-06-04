import QtQuick 2.10
import QtQuick.Controls 2.10 as QQC2
import org.kde.kirigami 2.12 as Kirigami

import "routes" as Routes
import "routes/entry" as EntryRoutes
import "routes/messages" as MessagesRoutes

import "qrc:/components" as Components

Kirigami.AbstractApplicationWindow {
    id: rootWindow

    property var aplayer: Components.AudioPlayer

    width: Kirigami.Units.gridUnit * 60
    height: Kirigami.Units.gridUnit * 35

    title: i18nc("window title", "Tok")

    color: content.settings.transparent ? "transparent" : Kirigami.Theme.backgroundColor

    onClosing: (e) => {
        content.closing(e)
    }

    Content { id: content; anchors.fill: parent }

}
