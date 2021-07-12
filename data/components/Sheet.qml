// SPDX-FileCopyrightText: 2020 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: AGPL-3.0-or-later

import QtQuick 2.15
import org.kde.kirigami 2.14 as Kirigami
import QtQuick.Controls 2.10 as QQC2

import org.kde.Tok 1.0 as Tok

QQC2.Dialog {
    modal: true
    padding: Kirigami.Units.gridUnit
    topPadding: Kirigami.Units.gridUnit
    leftPadding: Kirigami.Units.gridUnit * 2
    rightPadding: Kirigami.Units.gridUnit * 2
    bottomPadding: Kirigami.Units.gridUnit

    parent: rootRow

    x: Math.round((QQC2.Overlay.overlay.width / 2) - (this.width / 2))
    y: Math.round((QQC2.Overlay.overlay.height / 2) - (this.height / 2))

    height: contentItem.implicitHeight + topPadding + bottomPadding
}
