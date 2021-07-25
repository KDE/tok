// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.10
import org.kde.Tok 1.0 as Tok

QtObject {
    id: plaintext

    required property var messagesModel
    required property var userModel

    required property string chatID
    required property string messageID

    readonly property bool hasAuthor: universalData.data.authorKind === "user" && (plaintext.chatID != universalData.data.authorID)

    readonly property string timestamp: universalData.data.timestamp

    readonly property bool isOwn: tClient.ownID == universalData.data.authorID

    readonly property string authorName: tClient.ownID == universalData.data.authorID ? i18n("You") : authorData.data.name
    readonly property string onelinePlaintext: this.plaintext.split("\n")[0]
    readonly property string plaintext: {
        switch ([universalData.data.kind, universalData.dummy][0]) {
        case "messageText":
            return textData.data.content
        case "messageAnimation":
            return i18n("GIF")
        case "messageAudio":
            return i18n("Music")
        case "messageDocument":
            return i18nc("Specifically a file; I use Attachment in English because it sounds better than File in this context.", "Attachment")
        case "messagePhoto":
            return i18n("Photo")
        case "messageExpiredPhoto":
            return i18n("Expired Photo")
        case "messageSticker":
            return i18n("Sticker")
        case "messageVideo":
            return i18n("Video")
        case "messageExpiredVideo":
            return i18n("Expired Video")
        case "messageVideoNote":
            return i18n("Video Note")
        case "messageVoiceNote":
            return i18n("Voice Note")
        case "messageLocation":
            return i18n("Location")
        case "messageVenue":
            return i18n("Venue")
        case "messageContact":
            return i18n("Contact")
        case "messageDice":
            return i18n("Dice")
        case "messageGame":
            return i18n("Game")
        case "messagePoll":
            return i18n("Poll")
        case "messageInvoice":
            return i18n("Invoice")
        case "messageCall":
        case "messageVoiceChatStarted":
        case "messageVoiceChatEnded":
        case "messageInviteVoiceChatParticipants":
        case "messageBasicGroupChatCreate":
        case "messageSupergroupChatCreate":
        case "messageChatChangeTitle":
        case "messageChatChangePhoto":
        case "messageChatDeletePhoto":
        case "messageChatAddMembers":
        case "messageChatJoinByLink":
        case "messageChatDeleteMember":
        case "messageChatUpgradeTo":
        case "messageChatUpgradeFrom":
        case "messagePinMessage":
        case "messageScreenshotTaken":
        case "messageChatSetTtl":
        case "messageCustomServiceAction":
        case "messageGameScore":
        case "messagePaymentSuccessful":
        case "messageContactRegistered":
        case "messageWebsiteConnected":
        case "messagePassportDataSent":
        case "messageProximityAlertTriggered":
        case "messageUnsupported":
        default:
            return i18n("Unsupported")
        }
    }

    property var universalData: Tok.RelationalListener {
        id: universalData

        property string dummy

        model: plaintext.messagesModel
        key: [plaintext.chatID, plaintext.messageID]
        shape: QtObject {
            required property string authorID
            required property string authorKind
            required property string kind
            required property string timestamp
            required property string sendingState
        }
    }
    property var authorData: Tok.RelationalListener {
        id: authorData

        model: plaintext.userModel
        key: universalData.data.authorID

        shape: QtObject {
            required property string name
        }
    }
    property var textData: Tok.RelationalListener {
        id: textData

        model: plaintext.messagesModel
        key: [plaintext.chatID, plaintext.messageID]

        shape: QtObject {
            required property string content
        }
    }
}
