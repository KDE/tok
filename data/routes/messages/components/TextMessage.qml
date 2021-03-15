import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

QQC2.Control {
    topPadding: Kirigami.Units.largeSpacing
    bottomPadding: Kirigami.Units.largeSpacing
    leftPadding: Kirigami.Units.largeSpacing
    rightPadding: Kirigami.Units.largeSpacing

    background: Rectangle {
        radius: 4
        color: Kirigami.Theme.backgroundColor

        QQC2.Label {
            id: dummy
            text: " "
        }
        QQC2.Label {
            id: timestamp
            text: del.mTimestamp
            opacity: 0.5

            font.pointSize: -1
            font.pixelSize: Kirigami.Units.gridUnit * (2/3)
            anchors {
                bottom: parent.bottom
                right: parent.right
                margins: Kirigami.Units.smallSpacing
            }
        }
    }
    contentItem: ColumnLayout {
        QQC2.Label {
            text: userData.name
            color: Kirigami.NameUtils.colorsFromString(text)

            visible: del.separateFromPrevious && !(del.isOwnMessage && Kirigami.Settings.isMobile)

            wrapMode: Text.Wrap

            Layout.fillWidth: true
        }
        TextEdit {
            id: textEdit
            text: del.mContent + paddingT

            readonly property string paddingT: " ".repeat(Math.ceil(timestamp.implicitWidth / dummy.implicitWidth))

            readOnly: true
            selectByMouse: true
            wrapMode: Text.Wrap

            color: Kirigami.Theme.textColor
            selectedTextColor: Kirigami.Theme.highlightedTextColor
            selectionColor: Kirigami.Theme.highlightColor

            function clamp() {
                const l = length - paddingT.length
                if (selectionEnd >= l && selectionStart >= l) {
                    select(0, 0)
                } else if (selectionEnd >= l) {
                    select(selectionStart, l)
                } else if (selectionStart >= l) {
                    select(l, selectionEnd)
                }
            }

            onSelectionStartChanged: clamp()
            onSelectionEndChanged: clamp()

            Layout.fillWidth: true
        }
    }

    Layout.maximumWidth: del.recommendedSize
}