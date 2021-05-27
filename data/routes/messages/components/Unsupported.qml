import QtQuick 2.15
import org.kde.kirigami 2.12 as Kirigami

Kirigami.Icon {
    width: 64
    height: 64
    implicitWidth: 64
    implicitHeight: 64
    source: "application-x-core"

    TapHandler {
        onTapped: rootWindow.showPassiveNotification(messageData.data.kind)
    }
    // Accessible.name: `Unsupported message type.`
}