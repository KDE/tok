#pragma once

#include "qquickrelationallistener.h"

class QQmlRelationalListenerPrivate
{
    Q_DECLARE_PUBLIC(QQmlRelationalListener)
    QQmlRelationalListener* q_ptr;

public:
    QQmlRelationalListenerPrivate(QQmlRelationalListener* ptr)
        : q_ptr(ptr)
    {
    }

    QPointer<QAbstractRelationalModel> relationalModel = nullptr;
    QPointer<QQmlComponent> shape = nullptr;
    QVariant key = QVariant();
    QObject* dataObject = nullptr;
    bool complete = false;
};
