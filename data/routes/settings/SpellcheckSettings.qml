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
    Sonnet.Settings {
        id: settings
        onModifiedChanged: if (modified) save()
    }
    ColumnLayout {
        id: form

        anchors.fill: parent
        spacing: Kirigami.Units.smallSpacing

        ColumnLayout {
            QQC2.CheckBox {
                text: i18n("Enable automatic spellchecking")
                checked: settings.skipUppercase
                onToggled: settings.checkerEnabledByDefault = checked
            }
            QQC2.CheckBox {
                text: i18n("Ignore capitalised words")
                checked: settings.skipUppercase
                onToggled: settings.skipUppercase = checked
            }
            QQC2.CheckBox {
                text: i18n("Ignore hyphenated words")
                checked: settings.skipRunTogether
                onToggled: settings.skipRunTogether = checked
            }
            RowLayout {
                QQC2.Label {
                    text: i18n("Default language for spellchecking:")
                }
                QQC2.ComboBox {
                    model: settings.dictionaryModel
                    textRole: "display"
                    valueRole: "languageCode"

                    onActivated: settings.defaultLanguage = currentValue
                    Component.onCompleted: currentIndex = indexOfValue(settings.defaultLanguage)
                }
            }
        }
        Kirigami.Heading {
            level: 4
            text: i18n("Languages for spellchecking")
        }
        QQC2.CheckBox {
            text: i18n("Detect language to spellcheck automatically from below list")
            checked: settings.autodetectLanguage
            onToggled: settings.autodetectLanguage = checked
        }
        QQC2.ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            clip: false

            Kirigami.Theme.colorSet: Kirigami.Theme.View
            QQC2.ScrollBar.horizontal.policy: QQC2.ScrollBar.AlwaysOff

            background: Rectangle {
                border.width: 1
                border.color: Kirigami.ColorUtils.linearInterpolation(Kirigami.Theme.backgroundColor, Kirigami.Theme.textColor, 0.3)
                radius: 3
                anchors.fill: parent
                anchors.margins: -1
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
                        Kirigami.SearchField {
                            onTextChanged: dictModel.filterString = text

                            Layout.fillWidth: true
                        }
                    }
                }
                delegate: Kirigami.BasicListItem {
                    label: model.display
                    trailing: RowLayout {
                        QQC2.Label {
                            visible: model.isDefault
                            text: i18n("Default language")
                        }
                        QQC2.CheckBox {
                            checked: model.checked
                            onToggled: model.checked = checked
                        }
                    }
                }
            }
        }
    }

    Layout.fillHeight: true
    Layout.fillWidth: true
}
