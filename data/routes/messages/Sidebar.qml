import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import org.kde.kirigami 2.15 as Kirigami
import org.kde.kitemmodels 1.0

Kirigami.OverlayDrawer {
    enabled: true

    modal: false
    edge: Qt.application.layoutDirection == Qt.RightToLeft ? Qt.LeftEdge : Qt.RightEdge

    width: 200

    topPadding: 0
    leftPadding: 0
    rightPadding: 0
    Kirigami.Theme.colorSet: Kirigami.Theme.View
}