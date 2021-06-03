#define QT_QML_DEBUG

#include <QQmlEngine>
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
#include "utilities.h"
#include "chatsort.h"
#include "membersmodel.h"
#include "colorschemer.h"

#include "internallib/qquickrelationallistener.h"

#include "setup.h"

Q_DECLARE_METATYPE(QSharedPointer<TDApi::file>)

void performSetup(QQmlEngine* eng, bool testMode)
{
    Executor::instance();

    qRegisterMetaType<ChatsModel*>();
    qRegisterMetaType<MessagesModel*>();
    qRegisterMetaType<MessagesStore*>();
    qRegisterMetaType<UserDataModel*>();
    qRegisterMetaType<ChatsStore*>();
    qRegisterMetaType<MembersModel*>();
    qRegisterMetaType<Utilities*>();
    qRegisterMetaType<ColorSchemer*>();
    qRegisterMetaType<QSharedPointer<TDApi::file>>();
    qmlRegisterSingletonType<Utilities>("org.kde.Tok", 1, 0, "Utils", [](QQmlEngine*, QJSEngine*) -> QObject* { return new Utilities; });
    qmlRegisterSingletonType<ColorSchemer>("org.kde.Tok", 1, 0, "ColorSchemer", [](QQmlEngine*, QJSEngine*) -> QObject* { return new ColorSchemer; });
    qmlRegisterType<ChatSortModel>("org.kde.Tok", 1, 0, "ChatSortModel");
    qmlRegisterType<TokQmlRelationalListener>("org.kde.Tok", 1, 0, "RelationalListener");

    auto c = new Client(testMode);

    eng->rootContext()->setContextObject(new KLocalizedContext(eng));
    eng->rootContext()->setContextProperty("tClient", c);
    eng->addImageProvider("telegram", new TelegramImageProvider(c));
}
