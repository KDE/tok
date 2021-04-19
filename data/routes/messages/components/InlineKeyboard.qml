import QtQuick 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import org.kde.Tok 1.0 as Tok

import "qrc:/components" as GlobalComponents

ColumnLayout {
    Repeater {
        model: kbData.data.replyMarkupInlineKeyboard

        Repeater {
            model: modelData

            QQC2.Button {
                text: modelData

                Layout.fillWidth: true
            }
        }
    }
    Tok.RelationalListener {
        id: kbData

        model: tClient.messagesStore
        key: [del.mChatID, del.mID]
        shape: QtObject {
            required property var replyMarkupInlineKeyboard
        }
    }

    Layout.maximumWidth: del.recommendedSize
    Layout.leftMargin: Kirigami.Units.largeSpacing
    Layout.fillWidth: true
}