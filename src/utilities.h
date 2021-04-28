#pragma once

#include <QObject>

class Utilities : public QObject
{

    Q_OBJECT

public:

    Q_INVOKABLE bool isRTL(const QString& str);

};
