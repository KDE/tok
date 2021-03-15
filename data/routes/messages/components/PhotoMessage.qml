import QtQuick 2.10
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.12 as Kirigami

Image {
    source: del.mImageURL

    readonly property real ratio: width / implicitWidth

    smooth: true
    mipmap: true

    QQC2.Label {
        text: del.mTimestamp

        Kirigami.Theme.colorSet: Kirigami.Theme.Complementary
        Kirigami.Theme.inherit: false

        font.pointSize: -1
        font.pixelSize: Kirigami.Units.gridUnit * (2/3)

        padding: Kirigami.Units.smallSpacing
        leftPadding: Math.floor(Kirigami.Units.smallSpacing*(3/2))
        rightPadding: Math.floor(Kirigami.Units.smallSpacing*(3/2))

        anchors {
            bottom: parent.bottom
            right: parent.right
            margins: Kirigami.Units.largeSpacing
        }
        background: Rectangle {
            color: Kirigami.Theme.backgroundColor
            opacity: 0.7
            radius: 3
        }
    }

    Layout.preferredHeight: implicitHeight * ratio
    Layout.maximumWidth: del.recommendedSize
}
