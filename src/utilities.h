// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QQuickItem>

#include "yoinked from qt automotive/qivipendingreply.h"

class Utilities : public QObject
{

    Q_OBJECT

public:

    Q_INVOKABLE void setWindowPosition(QQuickWindow* win, int x, int y);
    Q_INVOKABLE bool isRTL(const QString& str);
    Q_INVOKABLE void setBlur(QQuickItem* item, bool doit);
    Q_INVOKABLE QString humanSize(int size);
    Q_INVOKABLE QIviPendingReplyBase pickFile(const QString& title, const QString& standardLocation);
    Q_INVOKABLE QString wordAt(int pos, const QString& in);

};
