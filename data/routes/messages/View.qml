import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

import QtQuick.Dialogs 1.0 as Dialogues

import "components" as Components
import "qrc:/components" as GlobalComponents

Kirigami.PageRoute {

name: "Messages/View"

Kirigami.ScrollablePage {
    id: messagesViewRoot

    property string chatID
    property string replyToID: ""
    property url uploadPath: ""
    property bool isPhoto: false

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
            required property string mTitle
            required property bool mCanSendMessages
        }
    }

    header: GlobalComponents.Header {
        RowLayout {
            QQC2.ToolButton {
                icon.name: "arrow-left"
                onClicked: Kirigami.PageRouter.bringToView(0)
                visible: !rootRow.wideMode
            }
            Kirigami.Heading {
                level: 4
                text: chatData.data.mTitle

                verticalAlignment: Text.AlignVCenter
                Layout.fillHeight: true
                Layout.margins: Kirigami.Units.largeSpacing
            }
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

                    placeholderText: enabled ? i18n("Write your message...") : i18n("You cannot send messages.")

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
