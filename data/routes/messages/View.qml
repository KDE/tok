import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok
import QtGraphicalEffects 1.15

import QtQuick.Dialogs 1.0 as Dialogues

import "components" as Components
import "qrc:/components" as GlobalComponents

Kirigami.PageRoute {

name: "Messages/View"
cache: true

Kirigami.ScrollablePage {
    id: messagesViewRoot

    property string chatID
    property string replyToID: ""
    property url uploadPath: ""
    property bool isPhoto: false

    function doit() {
        txtField.forceActiveFocus()
    }

    Dialogues.FileDialog {
        id: uploadDialog
        title: messagesViewRoot.isPhoto ? i18nc("Dialog title", "Upload photo") : i18nc("Dialog title", "Upload file")
        folder: messagesViewRoot.isPhoto ? shortcuts.pictures : shortcuts.home
        onAccepted: {
            messagesViewRoot.uploadPath = uploadDialog.fileUrl
            composeRow.send()
        }
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
            required property bool mCanSendMessages
        }
    }

    background: Rectangle {
        color: settings.transparent ? Kirigami.ColorUtils.scaleColor("transparent", {"alpha": -80}) : Kirigami.Theme.backgroundColor

        Loader {
            anchors.fill: parent
            active: settings.imageBackground | settings.transparent

            sourceComponent: Item {
                anchors.fill: parent
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
                name: chatData.data.mTitle
                source: chatData.data.mPhoto

                visible: !rootRow.shouldUseSidebars
                Layout.preferredHeight: backButton.implicitHeight
                Layout.preferredWidth: Layout.preferredHeight
            }
            Kirigami.Heading {
                level: 4
                text: chatData.data.mTitle

                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight

                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.margins: Kirigami.Units.largeSpacing
            }
            QQC2.ToolButton {
                icon.name: settings.userWantsSidebars ? "sidebar-collapse-right" : "sidebar-expand-right"
                onClicked: settings.userWantsSidebars = !settings.userWantsSidebars
                visible: rootRow.shouldUseSidebars
            }
            QQC2.ToolButton {
                icon.name: "documentinfo"
                onClicked: rootRow.layers.push(groupInfoComponent)
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

    footer: QQC2.ToolBar {
        ColumnLayout {
            anchors {
                left: parent.left
                right: parent.right
            }

            GlobalComponents.LoaderPlus {
                active: messagesViewRoot.replyToID != ""
                visible: messagesViewRoot.replyToID != ""

                sourceComponent: QQC2.Control {
                    padding: 6
                    contentItem: RowLayout {
                        spacing: 6

                        GlobalComponents.PlaintextMessage {
                            id: repliedToData

                            messagesModel: tClient.messagesStore
                            userModel: tClient.userDataModel
                            chatID: messagesViewRoot.chatID
                            messageID: messagesViewRoot.replyToID
                        }
                        Kirigami.Icon {
                            source: "dialog-messages"
                        }
                        ColumnLayout {
                            spacing: 1
                            QQC2.Label {
                                text: repliedToData.authorName
                                elide: Text.ElideRight
                                color: Kirigami.NameUtils.colorsFromString(repliedToData.authorName)
                                Layout.fillWidth: true
                            }
                            QQC2.Label {
                                text: repliedToData.onelinePlaintext
                                elide: Text.ElideRight
                                textFormat: TextEdit.MarkdownText
                                Layout.fillWidth: true
                            }

                            clip: true
                            Layout.fillWidth: true
                        }
                        QQC2.ToolButton {
                            icon.name: "dialog-cancel"
                            onClicked: messagesViewRoot.replyToID = ""
                        }

                        Layout.fillWidth: true
                    }
                    Layout.fillWidth: true
                }
            }

            RowLayout {
                id: composeRow

                function send() {
                    if (messagesViewRoot.uploadPath != "") {
                        if (messagesViewRoot.isPhoto) {
                            lView.model.sendPhoto(txtField.text, messagesViewRoot.uploadPath, messagesViewRoot.replyToID)
                        } else {
                            lView.model.sendFile(txtField.text, messagesViewRoot.uploadPath, messagesViewRoot.replyToID)
                        }

                        messagesViewRoot.uploadPath = ""
                        txtField.text = ""
                        messagesViewRoot.replyToID = ""
                        return
                    }
                    lView.model.send(txtField.text, messagesViewRoot.replyToID)
                    txtField.text = ""
                    messagesViewRoot.replyToID = ""
                }

                Layout.fillWidth: true

                QQC2.ToolButton {
                    Accessible.name: i18n("Upload photo")
                    icon.name: "photo"
                    onClicked: {
                        messagesViewRoot.isPhoto = true
                        uploadDialog.open()
                    }
                }
                QQC2.ToolButton {
                    Accessible.name: i18n("Upload file")
                    icon.name: "mail-attachment"
                    onClicked: {
                        messagesViewRoot.isPhoto = false
                        uploadDialog.open()
                    }
                }

                QQC2.TextArea {
                    id: txtField

                    background: null
                    enabled: chatData.data.mCanSendMessages
                    wrapMode: Text.Wrap

                    placeholderText: enabled ? i18n("Write your message…") : i18n("You cannot send messages.")

                    Tok.Clipboard.paste: function(clipboard) {
                        if (clipboard.hasUrls) {
                            messagesViewRoot.uploadPath = clipboard.urls[0]
                            composeRow.send()
                            return true
                        }
                    }

                    Keys.onReturnPressed: (event) => {
                        if (!(event.modifiers & Qt.ShiftModifier)) {
                            composeRow.send()
                            event.accepted = true
                        } else {
                            event.accepted = false
                        }
                    }
                    Keys.onTabPressed: nextItemInFocusChain().forceActiveFocus(Qt.TabFocusReason)
                    Layout.fillWidth: true
                }
                QQC2.Button {
                    Accessible.name: i18n("Send message")
                    icon.name: "document-send"
                    onClicked: composeRow.send()
                }
            }
        }
    }

    ListView {
        id: lView

        reuseItems: true
        verticalLayoutDirection: ListView.BottomToTop
        activeFocusOnTab: true

        property var visibleItems: []

        onVisibleItemsChanged: {
            lView.model.messagesInView(visibleItems)
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
