// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import Qt.labs.platform 1.1 as QQC2
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.10
import org.kde.Tok 1.0 as Tok
import org.kde.kirigami 2.15 as Kirigami

QQC2.Menu {
    title: i18nc("menu", "Create New")

    Sheet {
        id: nameGroup

        property string kind: ""
        property string codeKind: ""

        function doOpen(text, kind) {
            contactsPicker.model = tClient.newContactsModel()
            nameField.text = ""

            this.kind = text
            this.codeKind = kind
            this.open()
        }

        contentItem: ColumnLayout {
            spacing: Kirigami.Units.gridUnit

            Kirigami.Heading {
                text: nameGroup.kind
            }

            QQC2.TextField {
                id: nameField
                placeholderText: i18nc("placeholder text", "Name")

                Layout.fillWidth: true
            }

            QQC2.ScrollView {
                visible: nameGroup.codeKind === "privateGroup"

                Layout.preferredHeight: Kirigami.Units.gridUnit * 20
                Layout.fillWidth: true

                QQC2.ScrollBar.horizontal.policy: QQC2.ScrollBar.AlwaysOff

                ContactsPicker {
                    id: contactsPicker
                    isSelectMultiple: true
                    reuseItems: true
                }
            }

            RowLayout {
                Item { Layout.fillWidth: true }
                QQC2.Button {
                    text: i18n("Cancel")
                    onClicked: nameGroup.close()
                }
                QQC2.Button {
                    text: i18n("Create")
                    enabled: nameGroup.codeKind !== "privateGroup" || contactsPicker.model.selectedIDs.length > 0
                    onClicked: {
                        nameGroup.close()
                        tClient.chatsModel.createChat(nameField.text, nameGroup.codeKind, contactsPicker.model.selectedIDs)
                    }
                }
            }
        }
    }

    // QQC2.MenuItem {
    //     text: i18nc("menu", "Secret Chat…")
    // }
    QQC2.MenuItem {
        text: i18nc("menu", "Private Group…")
        onTriggered: nameGroup.doOpen(i18nc("dialog title", "Create a private group"), "privateGroup")
    }
    QQC2.MenuItem {
        text: i18nc("menu", "Public Group…")
        onTriggered: nameGroup.doOpen(i18nc("dialog title", "Create a public group"), "publicGroup")
    }
    QQC2.MenuItem {
        text: i18nc("menu", "Channel…")
        onTriggered: nameGroup.doOpen(i18nc("dialog title", "Create a broadcast channel"), "channel")
    }
}