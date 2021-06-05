// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QMap>

class Client;

class TestEventFeeder : public QObject
{

    Q_OBJECT

    Client* c;
    QMap<QString, std::function<void()>> stages;

private:

    void stageInitial();

    void stageEntryNumber();
    void stageEntryCode();
    void stageEntryPassword();

    void stageNoChats();

public:

    explicit TestEventFeeder(Client* parent = nullptr);
    Q_INVOKABLE void triggerStage(const QString& s);

};
