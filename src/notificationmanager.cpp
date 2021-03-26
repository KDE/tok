#include <KNotification>

#include "notificationmanager_p.h"

NotificationManager::NotificationManager(Client* c) : c(c), d(new Private)
{

}

NotificationManager::~NotificationManager()
{

}

#define match(n) { auto& _matchIt = n; switch(n->get_id()) {
#define endmatch } }

#define handle(c, v) case c::ID: { auto v = static_cast<c*>(_matchIt.get());
#define endhandle }

void NotificationManager::handleUpdateActiveNotifications(TDApi::object_ptr<TDApi::updateActiveNotifications> activeNotifications)
{
}

void NotificationManager::handleUpdateNotificationGroup(TDApi::object_ptr<TDApi::updateNotificationGroup> notificationGroup)
{
    for (auto& n : notificationGroup->added_notifications_) {
        using namespace TDApi;

        match (n->type_)
            handle (notificationTypeNewMessage, msg)
                match (msg->message_->content_)
                    handle (messageText, tmsg)
                        auto notif = new KNotification("newMessage");
                        notif->setText(QString::fromStdString(tmsg->text_->text_));
                        notif->setHint("resident", true);
                        notif->sendEvent();
                    endhandle
                endmatch
            endhandle
        endmatch

    }
}

void NotificationManager::handleUpdateNotification(TDApi::object_ptr<TDApi::updateNotification> notification)
{
}

void NotificationManager::handleUpdateHavePendingNotifications(TDApi::object_ptr<TDApi::updateHavePendingNotifications> havePendingNotification)
{
    Q_UNUSED(havePendingNotification)
}
