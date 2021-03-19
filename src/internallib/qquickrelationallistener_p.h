#pragma once

#include "qquickrelationallistener.h"

class TokQmlRelationalListenerPrivate
{
    Q_DECLARE_PUBLIC(TokQmlRelationalListener)
    TokQmlRelationalListener* q_ptr;

public:
    TokQmlRelationalListenerPrivate(TokQmlRelationalListener* ptr)
        : q_ptr(ptr)
    {
    }

    QPointer<TokAbstractRelationalModel> relationalModel = nullptr;
    QPointer<QQmlComponent> shape = nullptr;
    QVariant key = QVariant();
    QObject* dataObject = nullptr;
    bool complete = false;
};
