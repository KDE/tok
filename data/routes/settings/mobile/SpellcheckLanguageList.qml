// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.kitemmodels 1.0
import org.kde.Tok 1.0 as Tok

import org.kde.sonnet 1.0 as Sonnet
import org.kde.kitemmodels 1.0

import "qrc:/components" as GlobalComponents

Kirigami.ScrollablePage {
    topPadding: 0
    leftPadding: 0
    bottomPadding: 0
    rightPadding: 0

    titleDelegate: RowLayout {
        QQC2.ToolButton {
            icon.name: "arrow-left"
            onClicked: rootRow.layers.pop()
        }
        Kirigami.Heading {
            text: i18nc("title", "Spellchecking Languages")
            level: 4
        }
        Item { Layout.fillWidth: true }
    }

    Sonnet.Settings {
        id: settings
        onModifiedChanged: if (modified) save()
    }

    ListView {
        clip: true
        model: KSortFilterProxyModel {
            id: dictModel

            sourceModel: settings.dictionaryModel
            filterRole: "display"
        }
        footerPositioning: ListView.OverlayFooter
        footer: QQC2.ToolBar {
            z: 99
            width: tryit(() => parent.width, 0)
            position: QQC2.ToolBar.Footer
            contentItem: RowLayout {
                GlobalComponents.SearchField {
                    onTextChanged: dictModel.filterString = text

                    Layout.fillWidth: true
                }
            }
        }
        delegate: Kirigami.BasicListItem {
            label: model.display
            subtitle: model.isDefault ? i18n("Default language") : ""
            trailing: RowLayout {
                QQC2.CheckBox {
                    checked: model.checked
                    onToggled: model.checked = checked
                }
            }
        }
    }
}