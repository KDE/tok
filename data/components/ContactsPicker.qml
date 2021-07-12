// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import Qt.labs.platform 1.1 as Labs
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.10
import org.kde.Tok 1.0 as Tok
import org.kde.kirigami 2.15 as Kirigami

ListView {
    id: pickerView

    required property bool isSelectMultiple

    signal userSelected(string it)

    delegate: Kirigami.BasicListItem {
        id: del

        required property string userID
        required property bool isSelected

        leading: Kirigami.Avatar {
            name: userData.data.name
            source: userData.data.smallAvatar

            width: height
        }
        trailing: QQC2.CheckBox {
            enabled: false
            checked: del.isSelected
            visible: pickerView.isSelectMultiple
            anchors.verticalCenter: parent.verticalCenter
        }

        topPadding: Kirigami.Units.largeSpacing
        bottomPadding: Kirigami.Units.largeSpacing

        text: userData.data.name
        reserveSpaceForSubtitle: true

        onClicked: pickerView.isSelectMultiple ? pickerView.model.select(del.userID) : pickerView.userSelected(del.userID)

        Tok.RelationalListener {
            id: userData

            model: tClient.userDataModel
            key: del.userID
            shape: QtObject {
                required property string name
                required property string smallAvatar
            }
        }
    }
}
