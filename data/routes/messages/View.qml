// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.15 as Kirigami
import org.kde.Tok 1.0 as Tok
import QtGraphicalEffects 1.15

import "components" as Components
import "qrc:/components" as GlobalComponents

Kirigami.PageRoute {

name: "Messages/View"
cache: true

Kirigami.ScrollablePage {
    id: messagesViewRoot

    property string chatID
    property string interactionID: ""
    property string interactionKind: "reply"

    property url uploadPath: ""
    property bool isPhoto: false

    function doit() {
        composeBar.forceActiveFocus()
    }

    onChatIDChanged: {
        lView.model = tClient.messagesModel(messagesViewRoot.chatID)
    }

    Tok.RelationalListener {
        id: chatData

        model: tClient.chatsStore
        key: messagesViewRoot.chatID
        shape: QtObject {
            required property string mPhoto
            required property string mTitle
            required property string mKind
            required property string mKindID
            required property string mHeaderText
            required property var mCurrentActions
            required property bool mCanSendMessages
            required property bool mIsChannel
        }
    }

    background: Rectangle {
        color: settings.transparent ? Kirigami.ColorUtils.scaleColor("transparent", {"alpha": -80}) : Kirigami.Theme.backgroundColor

        Loader {
            anchors.fill: parent
            active: settings.imageBackground | settings.transparent

            sourceComponent: Item {
                anchors.fill: parent

                Loader {
                    anchors.fill: parent
                    active: settings.imageBackground
                    sourceComponent: Item {
                        Image {
                            id: bgImg

                            source: "qrc:/img/light background.png"
                            anchors.fill: parent
                            fillMode: Image.PreserveAspectCrop
                            visible: settings.imageBackground
                        }
                        FastBlur {
                            source: bgImg
                            anchors.fill: parent
                            cached: true
                            radius: 64
                            visible: settings.imageBackground
                        }
                    }
                }
                Rectangle {
                    color: Kirigami.Theme.backgroundColor
                    opacity: 0.5
                    anchors.fill: parent
                }
            }
        }
    }

    header: GlobalComponents.Header {
        RowLayout {
            Layout.leftMargin: !rootRow.shouldUseSidebars ? rootRow.leftOffset+Kirigami.Units.largeSpacing : Kirigami.Units.largeSpacing
            Layout.rightMargin: !rootRow.shouldUseSidebars ? rootRow.rightOffset+Kirigami.Units.largeSpacing : Kirigami.Units.largeSpacing

            QQC2.ToolButton {
                id: backButton
                icon.name: "arrow-left"
                onClicked: Kirigami.PageRouter.bringToView(0)
                visible: !rootRow.shouldUseSidebars
            }
            Kirigami.Avatar {
                id: theAvatar

                name: chatData.data.mTitle
                source: chatData.data.mPhoto

                visible: !rootRow.shouldUseSidebars
                Layout.preferredHeight: backButton.implicitHeight
                Layout.preferredWidth: Layout.preferredHeight

                TapHandler {
                    onTapped: {
                        let comp = groupInfoComponent.createObject(rootRow.layers)
                        comp.readyToDestroy.connect(() => {
                            hero.destination = null
                            comp.destroy()
                        })
                        hero.destination = comp.avatar
                        hero.open()
                    }
                }
            }
            QtObject {
                property Kirigami.Hero hero: Kirigami.Hero {
                    id: hero

                    source: theAvatar
                    destination: null
                }
            }
            ColumnLayout {
                spacing: 0

                QQC2.Label {
                    text: chatData.data.mTitle

                    elide: Text.ElideRight

                    Layout.fillWidth: true
                }
                QQC2.Label {
                    opacity: chatData.data.mCurrentActions.any ? 1.0 : 0.8
                    color: chatData.data.mCurrentActions.any ? Kirigami.Theme.focusColor : Kirigami.Theme.textColor
                    text: chatData.data.mCurrentActions.any ? chatData.data.mCurrentActions.message : chatData.data.mHeaderText
                }
                Layout.margins: Kirigami.Units.largeSpacing
                Layout.fillHeight: true
            }
            QQC2.ToolButton {
                icon.name: settings.userWantsSidebars ? "sidebar-collapse-right" : "sidebar-expand-right"
                onClicked: settings.userWantsSidebars = !settings.userWantsSidebars
                visible: rootRow.shouldUseSidebars
            }
            QQC2.ToolButton {
                icon.name: "documentinfo"
                onClicked: {
                    let comp = groupInfoComponent.createObject(rootRow.layers)
                    comp.readyToDestroy.connect(() => {
                        hero.destination = null
                        comp.destroy()
                    })
                    hero.destination = comp.avatar
                    hero.open()
                }
                visible: !rootRow.shouldUseSidebars
            }
        }
    }

    Component {
        id: groupInfoComponent

        Components.GroupInformation { anchors.fill: parent }
    }

    Loader {
        active: rootRow.shouldUseSidebars && settings.userWantsSidebars
        sourceComponent: QQC2.Drawer {
            width: 300
            height: rootWindow.height
            visible: true
            modal: false
            interactive: false
            position: 1
            edge: Qt.RightEdge

            background: Item {
                Kirigami.Separator {
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                        left: parent.left
                    }
                }
            }

            Components.GroupInformation { anchors.fill: parent }
        }
    }

    footer: Components.CompositionBar {
        id: composeBar
    }

    ListView {
        id: lView

        reuseItems: true
        verticalLayoutDirection: ListView.BottomToTop
        activeFocusOnTab: true

        property var visibleItems: []

        onVisibleItemsChanged: {
            if (!settings.ghostMode) lView.model.messagesInView(visibleItems)
        }

        delegate: Components.MessageDelegate {
            function add() {
                lView.visibleItems = [...lView.visibleItems, this.mID]
            }
            function remove() {
                lView.visibleItems = lView.visibleItems.filter(it => it != this.mID)
            }

            Component.onCompleted: add()
            ListView.onReused: add()

            Component.onDestruction: remove()
            ListView.onPooled: remove()
        }
    }
}

}
