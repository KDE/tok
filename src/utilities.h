#pragma once

#include <QObject>
#include <QQuickItem>

class Utilities : public QObject
{

    Q_OBJECT

public:

    Q_INVOKABLE bool isRTL(const QString& str);
    Q_INVOKABLE void setBlur(QQuickItem* item, bool doit);
    Q_INVOKABLE QString wordAt(int pos, const QString& in);

};
