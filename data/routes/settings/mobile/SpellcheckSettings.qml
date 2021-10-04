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
            text: i18nc("title", "Spellchecking Settings")
            level: 4
        }
        Item { Layout.fillWidth: true }
    }

    Sonnet.Settings {
        id: settings
        onModifiedChanged: if (modified) save()
    }

    ColumnLayout {
        spacing: 0

        Kirigami.Icon {
            source: "tools-check-spelling"

            Layout.preferredHeight: Kirigami.Units.iconSizes.huge
            Layout.preferredWidth: Kirigami.Units.iconSizes.huge
            Layout.margins: Kirigami.Units.gridUnit

            Layout.alignment: Qt.AlignHCenter
        }

        Kirigami.BasicListItem {
            text: i18nc("Mobile label; keep the text no longer than a few letters longer than the source string due to screen size constraints. Reword heavily if needed.", "Spellchecking enabled")
            onClicked: settings.checkerEnabledByDefault = !settings.checkerEnabledByDefault

            trailing: QQC2.CheckBox {
                checked: settings.checkerEnabledByDefault
                enabled: false
            }
        }
        Kirigami.BasicListItem {
            text: i18nc("Mobile label; keep the text no longer than a few letters longer than the source string due to screen size constraints. Reword heavily if needed.", "Ignore capitalised words")
            onClicked: settings.skipUppercase = !settings.skipUppercase

            trailing: QQC2.CheckBox {
                checked: settings.skipUppercase
                enabled: false
            }
        }
        Kirigami.BasicListItem {
            text: i18nc("Mobile label; keep the text no longer than a few letters longer than the source string due to screen size constraints. Reword heavily if needed.", "Ignore hyphenated words")
            onClicked: settings.skipRunTogether = !settings.skipRunTogether

            trailing: QQC2.CheckBox {
                checked: settings.skipRunTogether
                enabled: false
            }
        }
        Kirigami.BasicListItem {
            text: i18n("Set default language for spellchecking")
            subtitle: i18nc("currently <which language is default>", "Currently %1", settings.defaultLanguage)
            onClicked: rootRow.layers.push(Qt.resolvedUrl("SpellcheckDefault.qml"))

            trailing: Item {
                Kirigami.Icon {
                    source: "arrow-right"
                    height: Kirigami.Units.iconSizes.small
                    width: height

                    anchors {
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
        Kirigami.BasicListItem {
            text: i18n("Detect language from list automatically")
            onClicked: settings.autodetectLanguage = !settings.autodetectLanguage

            trailing: QQC2.CheckBox {
                checked: settings.autodetectLanguage
                enabled: false
            }
        }
        Kirigami.BasicListItem {
            text: i18n("Language list")
            subtitle: i18n("Select languages for automatic detection")
            onClicked: rootRow.layers.push(Qt.resolvedUrl("SpellcheckLanguageList.qml"))

            trailing: Item {
                Kirigami.Icon {
                    source: "arrow-right"
                    height: Kirigami.Units.iconSizes.small
                    width: height

                    anchors {
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
        Kirigami.BasicListItem {
            text: i18n("Spellcheck dictionary")
            subtitle: i18n("Add and remove words from your dictionary")
            onClicked: rootRow.layers.push(Qt.resolvedUrl("SpellcheckDictionary.qml"))

            trailing: Item {
                Kirigami.Icon {
                    source: "arrow-right"
                    height: Kirigami.Units.iconSizes.small
                    width: height

                    anchors {
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
   }
}