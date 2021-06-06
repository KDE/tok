#pragma once

#include <QObject>
#include <QQuickItem>

class Utilities : public QObject
{

    Q_OBJECT

public:

    Q_INVOKABLE bool isRTL(const QString& str);
    Q_INVOKABLE void setBlur(QQuickItem* item, bool doit);
    // returns a rich-text version of the input string with
    // stuff done as a rich-text label with emoji support
    Q_INVOKABLE QString emojified(const QString& in);

};
