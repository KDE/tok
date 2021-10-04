// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.10
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.3 as QQC2
import QtQuick.Window 2.2
import org.kde.Tok 1.0 as Tok

import org.kde.kirigami 2.14 as Kirigami

import org.kde.sonnet 1.0 as Sonnet
import org.kde.kitemmodels 1.0

Item {
    readonly property bool noMargin: true

    Sonnet.Settings {
        id: settings
        onModifiedChanged: if (modified) save()
    }
    ColumnLayout {
        id: form

        anchors.fill: parent
        spacing: 0

        Kirigami.Heading {
            level: 4
            text: i18n("Dictionary for spellchecking")
            Layout.margins: Kirigami.Units.smallSpacing
        }
        Kirigami.Separator {
            Layout.fillWidth: true
        }
        QQC2.ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Kirigami.Theme.colorSet: Kirigami.Theme.View
            QQC2.ScrollBar.horizontal.policy: QQC2.ScrollBar.AlwaysOff

            background: Rectangle {
                color: Kirigami.Theme.backgroundColor
            }

            ListView {
                id: dictListView

                clip: true
                model: settings.currentIgnoreList

                function remove(word) {
                    settings.currentIgnoreList = settings.currentIgnoreList.filter((value) => value !== word)
                }

                footerPositioning: ListView.OverlayFooter
                footer: QQC2.ToolBar {
                    z: 99
                    width: tryit(() => parent.width, 0)
                    position: QQC2.ToolBar.Footer
                    contentItem: RowLayout {
                        Kirigami.SearchField {
                            onTextChanged: dictModel.filterString = text

                            Layout.fillWidth: true
                        }
                    }
                }
                delegate: Kirigami.BasicListItem {
                    label: model.modelData
                    reserveSpaceForLabel: true
                    trailing: QQC2.Button {
                        icon.name: "delete"
                        onClicked: dictListView.remove(model.modelData)
                    }
                }
            }
        }
    }

    Layout.fillHeight: true
    Layout.fillWidth: true
}
