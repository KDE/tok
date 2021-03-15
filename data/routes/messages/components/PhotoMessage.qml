import QtQuick 2.10
import QtQuick.Layouts 1.10

Image {
    source: del.mImageURL

    readonly property real ratio: width / implicitWidth

    smooth: true
    mipmap: true

    Layout.preferredHeight: implicitHeight * ratio
    Layout.maximumWidth: del.recommendedSize
}
