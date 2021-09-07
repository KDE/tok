#pragma once

#include <QJSValue>
#include <QObject>
#include <QQuickItem>

#include "client.h"

class InstantViewer : public QObject
{

    Q_OBJECT

public:
    Q_INVOKABLE void doInstantView(const QString& url, Client* c, QQuickItem* item);

};
