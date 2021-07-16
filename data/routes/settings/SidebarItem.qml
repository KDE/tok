// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.3 as QQC2
import QtQuick.Window 2.2

import org.kde.kirigami 2.12 as Kirigami

QQC2.ItemDelegate {
    id: del

    function activated() {
        pageLoader.source = this.page
    }

    required property string pageName
    required property string accessibleDescription
    required property url page

    highlighted: pageLoader.source === page

    Accessible.role: Accessible.MenuItem
    Accessible.name: del.pageName
    Accessible.description: del.accessibleDescription

    Layout.preferredWidth: Kirigami.Units.gridUnit * 7
    hoverEnabled: true

    onClicked: {
        if (highlighted) {
            return
        }

        activated()
    }

    contentItem: ColumnLayout {
        spacing: Kirigami.Units.smallSpacing

        Kirigami.Icon {
            Layout.preferredWidth: Kirigami.Units.iconSizes.medium
            Layout.preferredHeight: width

            source: del.icon.name
            selected: del.highlighted

            Layout.alignment: Qt.AlignHCenter
        }

        QQC2.Label {
            text: del.pageName
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter

            Layout.fillWidth: true
            Layout.leftMargin: Kirigami.Units.smallSpacing
            Layout.rightMargin: Kirigami.Units.smallSpacing
        }
    }
}
