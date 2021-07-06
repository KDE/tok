// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

import "qrc:/components" as Components

QQC2.Control {
    visible: webPageData.data.hasWebPage
    topPadding: 0
    leftPadding: 8
    rightPadding: 0
    bottomPadding: 0

    Tok.RelationalListener {
        id: webPageData

        model: tClient.messagesStore
        key: [del.mChatID, del.mID]
        shape: QtObject {
            required property bool hasWebPage
            required property string webPageURL
            required property string webPageDisplay
            required property string webPageSiteName
            required property string webPageTitle
            required property string webPageText
            required property bool hasInstantView
        }
    }

    background: Item {
        Rectangle {
            anchors {
                left: parent.left
                top: parent.top
                bottom: parent.bottom
            }
            width: 2
            color: Kirigami.Theme.focusColor
        }
    }
    contentItem: ColumnLayout {
        id: replyCol

        spacing: 1
        QQC2.Label {
            text: webPageData.data.webPageTitle
            elide: Text.ElideRight
            color: Kirigami.Theme.focusColor

            Layout.fillWidth: true
        }
        QQC2.Label {
            text: webPageData.data.webPageText.split("\n")[0]
            elide: Text.ElideRight

            Layout.fillWidth: true
        }

        QQC2.ToolButton {
            visible: false // TODO: implement
            text: i18nc("button that opens a preview of a web page. 'instant view' is sort of like a brand name, but also sort of not? i would recommend consulting translations.telegram.org to see how instant view was translated officially.", "Instant View")

            Layout.fillWidth: true
        }

        clip: true
    }

    Layout.fillWidth: true
    Layout.topMargin: 3
}
