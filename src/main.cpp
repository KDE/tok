// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#define QT_QML_DEBUG

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlProperty>
#include <QMediaPlayer>
#include <QWindow>

#include <KLocalizedString>
#include <KDBusService>
#include <KWindowSystem>

#include "setup.h"
#include "mprissetup.h"

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setApplicationName("org.kde.Tok");
    QCoreApplication::setOrganizationName("KDE");

    QApplication app(argc, argv);
    app.setDesktopFileName("org.kde.Tok.desktop");
    KLocalizedString::setApplicationDomain("tok");

    KDBusService service(KDBusService::Unique);

    QQmlApplicationEngine engine;

    performSetup(&engine, false);

    const QUrl url(!qgetenv("TOK_MAUI").isEmpty() ? QStringLiteral("qrc:/maui_main.qml") : QStringLiteral("qrc:/main.qml"));
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

    auto aplayer = engine.rootObjects()[0]->property("aplayer").value<QObject*>();
    auto it = aplayer->property("mediaObject").value<QMediaPlayer*>();
    auto window = qobject_cast<QWindow*>(engine.rootObjects()[0]);
    setupMPRIS(&app, aplayer, it);

    QObject::connect(&service, &KDBusService::activateRequested, [=]() {
        window->show();
        KWindowSystem::updateStartupId(window);
        window->raise();
        KWindowSystem::activateWindow(window);
    });

    return QApplication::exec();
}
