// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QQuickItem>
#include <QJsonObject>

#include "yoinked from qt automotive/qivipendingreply.h"

class Utilities : public QObject
{

    Q_OBJECT

public:

    Q_INVOKABLE bool isRTL(const QString& str);
    Q_INVOKABLE void setBlur(QQuickItem* item, bool doit);
    Q_INVOKABLE QJsonObject fileData(const QString& data);
    Q_INVOKABLE QString typeOfFile(const QUrl& url);
    Q_INVOKABLE QString fileIcon(const QUrl& url);
    Q_INVOKABLE QString humanSize(int size);
    Q_INVOKABLE QIviPendingReplyBase pickFile(const QString& title, const QString& standardLocation);
    Q_INVOKABLE QString wordAt(int pos, const QString& in);

};
