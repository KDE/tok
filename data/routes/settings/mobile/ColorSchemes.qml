// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.kitemmodels 1.0
import org.kde.Tok 1.0 as Tok

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
            text: i18nc("title", "Pick a color scheme…")
            level: 4
        }
        Item { Layout.fillWidth: true }
    }

    ListView {
        spacing: 0

        model: Tok.ColorSchemer.model

        delegate: Kirigami.BasicListItem {
            required property int index
            required property string colorSchemeName

            text: colorSchemeName

            onClicked: {
                Tok.ColorSchemer.apply(index)
                rootRow.layers.pop()
            }
        }
    }
}