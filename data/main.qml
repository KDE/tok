import QtQuick 2.10
import QtQuick.Controls 2.10 as QQC2
import org.kde.kirigami 2.12 as Kirigami

import "routes" as Routes
import "routes/entry" as EntryRoutes
import "routes/messages" as MessagesRoutes

Kirigami.AbstractApplicationWindow {
    id: rootWindow

    title: i18nc("window title", "Tok")

    Content { anchors.fill: parent }

}
