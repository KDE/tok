import QtQuick 2.10
import org.kde.kirigami 2.13 as Kirigami
import QtQuick.Controls 2.10 as QQC2

Loader {
    id: loadRoot

    Component {
        id: regularMenu

        QQC2.Menu {
            Repeater {
                model: loadRoot.items

                delegate: QQC2.MenuItem {
                    text: modelData.text
                    visible: modelData.visible
                    enabled: modelData.enabled
                    onTriggered: modelData.triggered()
                }
            }
        }
    }
    Component {
        id: mobileMenu

        Kirigami.OverlayDrawer {
            height: (Kirigami.Units.gridUnit*3) * loadRoot.items.length
            edge: Qt.BottomEdge
            padding: 0
            leftPadding: 0
            rightPadding: 0
            bottomPadding: 0
            topPadding: 0

            ListView {
                model: loadRoot.items
                anchors.fill: parent
                clip: true
                currentIndex: -1

                delegate: Kirigami.AbstractListItem {
                    contentItem: Kirigami.Heading {
                        level: 3
                        text: modelData.text
                    }
                    enabled: modelData.enabled
                    visible: modelData.visible
                    onClicked: {
                        modelData.triggered()
                        loadRoot.item.close()
                    }
                    implicitHeight: (Kirigami.Units.gridUnit*3)
                }
            }
        }
    }

    default property list<ResponsiveMenuItem> items
    asynchronous: true
    sourceComponent: Kirigami.Settings.isMobile ? mobileMenu : regularMenu

    function open() {
        if (Kirigami.Settings.isMobile)
            item.open()
        else
            item.popup()
    }
}
