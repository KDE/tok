// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#define QT_QML_DEBUG

#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickWindow>

#include <KLocalizedContext>

#include "chatsstore.h"
#include "chatsmodel.h"
#include "messagesmodel.h"
#include "client.h"
#include "keys.h"
#include "tgimageprovider.h"
#include "userdata.h"
#include "util.h"
#include "utilities.h"
#include "filemangler.h"
#include "chatsort.h"
#include "copyinterceptor.h"
#include "membersmodel.h"
#include "colorschemer.h"
#include "usersortmodel.h"
#include "searchmessagesmodel.h"
#include "chatlistmodel.h"
#include "proxymodel.h"
#include "stickersetsmodel.h"
#include "stickersetsstore.h"

#include "internallib/qquickrelationallistener.h"

#include "setup.h"
#include "contactsmodel.h"

Q_IMPORT_PLUGIN(LottiePlugin)

Q_DECLARE_METATYPE(QSharedPointer<TDApi::file>)

void performSetup(QQmlEngine* eng, bool testMode)
{
    Executor::instance();

    qRegisterMetaType<ChatsModel*>();
    qRegisterMetaType<Span>();
    qRegisterMetaType<MessagesModel*>();
    qRegisterMetaType<MessagesStore*>();
    qRegisterMetaType<UserDataModel*>();
    qRegisterMetaType<ChatsStore*>();
    qRegisterMetaType<MembersModel*>();
    qRegisterMetaType<Utilities*>();
    qRegisterMetaType<ColorSchemer*>();
    qRegisterMetaType<FileMangler*>();
    qRegisterMetaType<ProxyModel*>();
    qRegisterMetaType<SearchMessagesModel*>();
    qRegisterMetaType<ChatListModel*>();
    qRegisterMetaType<ContactsModel*>();
    qRegisterMetaType<Sticker>();
    qRegisterMetaType<StickerSetsModel*>();
    qRegisterMetaType<StickerSetsStore*>();
    qRegisterMetaType<QSharedPointer<TDApi::file>>();
    qmlRegisterSingletonType<Utilities>("org.kde.Tok", 1, 0, "Utils", [](QQmlEngine*, QJSEngine*) -> QObject* { return new Utilities; });
    qmlRegisterSingletonType<ColorSchemer>("org.kde.Tok", 1, 0, "ColorSchemer", [](QQmlEngine*, QJSEngine*) -> QObject* { return new ColorSchemer; });
    qmlRegisterUncreatableType<CopyInterceptor>("org.kde.Tok", 1, 0, "Clipboard", "You cannot create an instance of Clipboard.");
    qmlRegisterUncreatableType<Client>("org.kde.Tok", 1, 0, "Client", "please use the tClient singleton");
    qmlRegisterType<ChatSortModel>("org.kde.Tok", 1, 0, "ChatSortModel");
    qmlRegisterType<UserSortModel>("org.kde.Tok", 1, 0, "UserSortModel");
    qmlRegisterType<TokQmlRelationalListener>("org.kde.Tok", 1, 0, "RelationalListener");

    QQuickWindow::setDefaultAlphaBuffer(true);

    auto c = new Client(testMode);

    eng->rootContext()->setContextObject(new KLocalizedContext(eng));
    eng->rootContext()->setContextProperty("tClient", c);
    eng->addImageProvider("telegram", new TelegramImageProvider(c));
}
