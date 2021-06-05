// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QtQuickTest>
#include <QQmlEngine>
#include <QQmlContext>

#include "client.h"
#include "setup.h"
#include "test_event_feeder.h"

class Setup : public QObject
{
Q_OBJECT

public:
    Setup() {}

public Q_SLOTS:
    void qmlEngineAvailable(QQmlEngine *engine)
    {
        performSetup(engine, true);

        engine->rootContext()->setContextProperty("testEventFeeder", new TestEventFeeder(qobject_cast<Client*>(engine->rootContext()->contextProperty("tClient").value<QObject*>())));

        qmlRegisterType(QUrl("qrc:/main.qml"), "org.kde.Tok.Tests", 1, 0, "MainWindow");
    }
};

QUICK_TEST_MAIN_WITH_SETUP(TokTest, Setup)

#include "test_main.moc"
