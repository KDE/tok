#include <KNotification>
#include <KNotificationReplyAction>
#include <KLocalizedString>

#include "extractinator.h"
#include "notificationmanager_p.h"

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
    for (auto& n : notificationGroup->added_notifications_) {
        using namespace TDApi;

        KNotification *notif = nullptr;

        match (n->type_)
            handleCase (notificationTypeNewMessage, msg)

                notif = new KNotification("newMessage");
                auto [title, body] = Extractinator::extract(c, msg->message_.get());
                notif->setTitle(title);
                notif->setText(body);

                auto reply = std::make_unique<KNotificationReplyAction>(i18n("Reply"));
                reply->setPlaceholderText(i18n("Reply to %1...", title));
                QObject::connect(reply.get(), &KNotificationReplyAction::replied, c, [this, chatID = msg->message_->chat_id_](const QString& reply) {
                    qDebug() << "replied!";

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
