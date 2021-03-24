#pragma once

#include "client.h"
#include "defs.h"

class NotificationManager
{
    Client* c;

    struct Private;
    std::unique_ptr<Private> d;

public:
    explicit NotificationManager(Client* c);
    ~NotificationManager();

    void handleUpdateActiveNotifications(TDApi::object_ptr<TDApi::updateActiveNotifications> activeNotifications);
    void handleUpdateNotificationGroup(TDApi::object_ptr<TDApi::updateNotificationGroup> notificationGroup);
    void handleUpdateNotification(TDApi::object_ptr<TDApi::updateNotification> notification);
    void handleUpdateHavePendingNotifications(TDApi::object_ptr<TDApi::updateHavePendingNotifications> havePendingNotification);

};
