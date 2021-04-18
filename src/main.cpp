// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <KLocalizedContext>

#include "chatsstore.h"
#include "chatsmodel.h"
#include "messagesmodel.h"
#include "client.h"
#include "keys.h"
#include "tgimageprovider.h"
#include "userdata.h"
#include "util.h"

#include "internallib/qquickrelationallistener.h"

Q_DECLARE_METATYPE(QSharedPointer<TDApi::file>)

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setApplicationName("org.kde.Tok");
    QCoreApplication::setOrganizationName("KDE");

    Executor::instance();

    QApplication app(argc, argv);
    app.setDesktopFileName("org.kde.Tok.desktop");

    qRegisterMetaType<ChatsModel*>();
    qRegisterMetaType<MessagesModel*>();
    qRegisterMetaType<MessagesStore*>();
    qRegisterMetaType<UserDataModel*>();
    qRegisterMetaType<ChatsStore*>();
    qRegisterMetaType<QSharedPointer<TDApi::file>>();
    qmlRegisterType<TokQmlRelationalListener>("org.kde.Tok", 1, 0, "RelationalListener");

    QQmlApplicationEngine engine;

    auto c = new Client;

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.rootContext()->setContextProperty("tClient", c);
    engine.addImageProvider("telegram", new TelegramImageProvider(c));

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject* obj, const QUrl& objUrl) {
            if ((obj == nullptr) && url == objUrl) {
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);
    engine.load(url);

    return QApplication::exec();
}
