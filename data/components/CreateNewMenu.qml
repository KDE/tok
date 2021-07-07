// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import Qt.labs.platform 1.1 as Labs
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.10
import org.kde.Tok 1.0 as Tok
import org.kde.kirigami 2.15 as Kirigami

Labs.Menu {
    title: i18nc("menu", "Create New")

    Sheet {
        id: nameGroup

        property string kind: ""
        property string codeKind: ""

        function doOpen(text, kind) {
            nameField.text = ""

            this.kind = text
            this.codeKind = kind
            this.open()
        }

        parent: rootRow

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

            RowLayout {
                Item { Layout.fillWidth: true }
                QQC2.Button {
                    text: i18n("Cancel")
                    onClicked: nameGroup.close()
                }
                QQC2.Button {
                    text: i18n("Create")
                    onClicked: {
                        nameGroup.close()
                        tClient.chatsModel.createChat(nameField.text, nameGroup.codeKind)
                    }
                }
            }
        }
    }

    // Labs.MenuItem {
    //     text: i18nc("menu", "Secret Chat…")
    // }
    // Labs.MenuItem {
    //     text: i18nc("menu", "Private Group…")
    //     onTriggered: nameGroup.doOpen(i18nc("dialog title", "Create a private group"), "privateGroup")
    // }
    Labs.MenuItem {
        text: i18nc("menu", "Public Group…")
        onTriggered: nameGroup.doOpen(i18nc("dialog title", "Create a public group"), "publicGroup")
    }
    Labs.MenuItem {
        text: i18nc("menu", "Channel…")
        onTriggered: nameGroup.doOpen(i18nc("dialog title", "Create a broadcast channel"), "channel")
    }
}