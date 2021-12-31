// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtGraphicalEffects 1.15
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok
import org.kde.kquickchatcomponents 1.0 as KQCC

KQCC.ChatBubble {
    QQC2.Label {
        id: dummy
        text: " "
    }

    tailVisible: del.showAvatar
    readonly property string textPadding: " ".repeat(Math.ceil(inlineFooter.width / dummy.implicitWidth)) + "⠀"

    inlineFooterContent: [
        KQCC.Timestamp {
            text: messageData.data.timestamp
            edited: messageData.data.editedTimestamp !== ""
            icon: {
                if (messageData.data.authorID === tClient.ownID)
                    return ""

                const states = {
                    "pending": "clock",
                    "failed": "emblem-error",
                    "sent": "emblem-ok-symbolic",
                }
                return states[messageData.data.sendingState]
            }
        }
    ]
}
