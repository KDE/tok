import QtQuick 2.15
import QtQuick.Layouts 1.10
import org.kde.kirigami 2.10 as Kirigami

Kirigami.AbstractApplicationHeader {
    preferredHeight: Math.max(colLayout.implicitHeight, Math.round(Kirigami.Units.gridUnit * 2.5))

    default property alias it: colLayout.data

    DragHandler {
        acceptedDevices: PointerDevice.GenericPointer
        grabPermissions: PointerHandler.CanTakeOverFromItems | PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByAnything
        onActiveChanged: if (active) rootWindow.startSystemMove()
    }

    contentItem: ColumnLayout {
        id: colLayout
        anchors.fill: parent
    }
}