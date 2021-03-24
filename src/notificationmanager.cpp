#include <KNotification>

#include "notificationmanager_p.h"

NotificationManager::NotificationManager(Client* c) : c(c), d(new Private)
{

}

NotificationManager::~NotificationManager()
{

}

void NotificationManager::handleUpdateActiveNotifications(TDApi::object_ptr<TDApi::updateActiveNotifications> activeNotifications)
{
    qDebug() << "do sound";

    auto notif = new KNotification("newMessage");
    notif->setText("eww");
    notif->sendEvent();
}

void NotificationManager::handleUpdateNotificationGroup(TDApi::object_ptr<TDApi::updateNotificationGroup> notificationGroup)
{
    qDebug() << "update notification group";
}

void NotificationManager::handleUpdateNotification(TDApi::object_ptr<TDApi::updateNotification> notification)
{
    qDebug() << "update notification";
}

void NotificationManager::handleUpdateHavePendingNotifications(TDApi::object_ptr<TDApi::updateHavePendingNotifications> havePendingNotification)
{
    Q_UNUSED(havePendingNotification)
}
