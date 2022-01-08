// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.kitemmodels 1.0
import org.kde.Tok 1.0 as Tok
import "qrc:/components" as Components

QQC2.Popup {
    id: _popup

    onVisibleChanged: {
        if (!visible) {
            return
        }
        quickSearch.forceActiveFocus()
        quickSearch.text = ""
    }
    anchors.centerIn: QQC2.Overlay.overlay
    background: Kirigami.Card {}
    height: 2 * Math.round(implicitHeight / 2)
    padding: Kirigami.Units.largeSpacing * 2
    contentItem: ColumnLayout {
        spacing: Kirigami.Units.largeSpacing * 2

        Components.SearchField {
            id: quickSearch

            Layout.preferredWidth: 400
            Keys.onLeftPressed: cView.decrementCurrentIndex()
            Keys.onRightPressed: cView.incrementCurrentIndex()
            onAccepted: {
                _popup.visible = false
                lView.triggerPage(cView.itemAtIndex(cView.currentIndex).mID)
            }
        }
        ListView {
            id: cView

            orientation: Qt.Horizontal
            spacing: Kirigami.Units.largeSpacing

            model: Tok.ChatSortModel {
                sourceModel: tClient.chatsModel
                store: tClient.chatsStore
                filter: quickSearch.text
            }

            Layout.preferredHeight: 64
            Layout.fillWidth: true

            delegate: Kirigami.Avatar {
                id: del

                implicitHeight: 64
                implicitWidth: 64

                required property string mID

                name: chatData.data.mTitle
                source: chatData.data.mPhoto

                Tok.RelationalListener {
                    id: chatData

                    model: tClient.chatsStore
                    key: del.mID
                    shape: QtObject {
                        required property string mTitle
                        required property string mPhoto
                    }
                }
            }
        }
    }
    modal: true
    focus: true
}
