// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <KNotification>
#include <KNotificationReplyAction>
#include <KLocalizedString>

#include "extractinator.h"
#include "notificationmanager_p.h"
#include "chatsstore.h"
#include "chatsstore_p.h"

NotificationManager::NotificationManager(Client* c) : c(c), d(new Private)
{

}

NotificationManager::~NotificationManager()
{

}

void NotificationManager::handleUpdateActiveNotifications(TDApi::object_ptr<TDApi::updateActiveNotifications> activeNotifications)
{
}

void NotificationManager::handleUpdateNotificationGroup(TDApi::object_ptr<TDApi::updateNotificationGroup> notificationGroup)
{
    if (c->doNotDisturb()) {
        return;
    }
    for (auto& n : notificationGroup->added_notifications_) {
        using namespace TDApi;

        KNotification *notif = nullptr;

        match (n->type_)
            handleCase (notificationTypeNewMessage, msg)

                notif = new KNotification("newMessage");
                auto [author, body] = Extractinator::extract(c, msg->message_.get());
                notif->setText(QString("<b>%1</b>\n%2").arg(author, body));

                if (c->chatsStore()->d->chatData.contains(msg->message_->chat_id_)) {
                    const auto& data = c->chatsStore()->d->chatData[msg->message_->chat_id_];
                    notif->setTitle(QString::fromStdString(data->title_));

                    if (data->type_->get_id() == chatTypePrivate::ID || data->type_->get_id() == chatTypeSecret::ID) {
                        notif->setTitle(author);
                        notif->setText(body);
                    }

                    if (!data->photo_) {
                        goto brk;
                    }

                    QPixmap pix;

                    if (data->photo_->small_->local_->is_downloading_completed_) {
                        pix = QPixmap(QString::fromStdString(data->photo_->small_->local_->path_), "jpeg");
                    } else if (data->photo_->big_->local_->is_downloading_completed_) {
                        pix = QPixmap(QString::fromStdString(data->photo_->big_->local_->path_), "jpeg");
                    } else if (data->photo_->minithumbnail_) {
                        QString img("data:image/jpg;base64,");
                        auto ba = QByteArray::fromStdString(data->photo_->minithumbnail_->data_);
                        img.append(QString::fromLatin1(ba.toBase64().data()));

                        pix = QPixmap(img);
                    } else {
                        goto brk;
                    }

                    notif->setPixmap(pix);
                } else {
                    notif->setTitle(author);
                    notif->setText(body);
                }
                brk:

                auto reply = std::make_unique<KNotificationReplyAction>(i18nc("button action", "Reply"));
                reply->setPlaceholderText(i18nc("%1 is the author name of the message to reply to", "Reply to %1…", author));
                QObject::connect(reply.get(), &KNotificationReplyAction::replied, c, [this, chatID = msg->message_->chat_id_](const QString& reply) {
                    auto send_message = TDApi::make_object<TDApi::sendMessage>();
                    send_message->chat_id_ = chatID;
                    auto message_content = TDApi::make_object<TDApi::inputMessageText>();
                    message_content->text_ = TDApi::make_object<TDApi::formattedText>();
                    message_content->text_->text_ = reply.toStdString();
                    send_message->input_message_content_ = std::move(message_content);

                    c->callP<TDApi::sendMessage>( [=](TDApi::sendMessage::ReturnType t) {}, std::move(send_message) );
                });
                notif->setReplyAction(std::move(reply));

            endhandle
        endmatch

        if (notif != nullptr) {
            notif->sendEvent();
        }
    }
}

void NotificationManager::handleUpdateNotification(TDApi::object_ptr<TDApi::updateNotification> notification)
{
}

void NotificationManager::handleUpdateHavePendingNotifications(TDApi::object_ptr<TDApi::updateHavePendingNotifications> havePendingNotification)
{
    Q_UNUSED(havePendingNotification)
}
