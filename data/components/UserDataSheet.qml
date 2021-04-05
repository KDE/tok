// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: AGPL-3.0-or-later

import QtQuick 2.10
import QtQuick.Layouts 1.10
import org.kde.kirigami 2.14 as Kirigami
import QtQuick.Controls 2.10 as QQC2
import org.kde.Tok 1.0 as Tok

Loader {
    id: __loader

    active: false
    required property string userID

    function open() {
        if (!active) {
            active = true
        } else {
            this.item.open()
        }
    }

    onLoaded: this.item.open()
    sourceComponent: Kirigami.OverlaySheet {
        id: __overlaySheet

        parent: rootWindow

        property Tok.RelationalListener mu: Tok.RelationalListener {
            id: userData

            model: tClient.userDataModel
            key: __loader.userID
            shape: QtObject {
                required property string name
                required property string smallAvatar
                required property string bio
            }
        }

        ColumnLayout {
            Kirigami.Avatar {
                name: userData.data.name
                source: userData.data.smallAvatar
                implicitHeight: 64
                implicitWidth: 64

                Layout.alignment: Qt.AlignHCenter
            }
            Kirigami.Heading {
                level: 2

                text: userData.data.name
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignHCenter

                Layout.fillWidth: true
            }
            QQC2.Label {
                text: userData.data.bio
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignHCenter

                Layout.fillWidth: true
            }
        }
    }
}
