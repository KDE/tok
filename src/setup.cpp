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
#include "chatsort.h"

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
    qRegisterMetaType<QSharedPointer<TDApi::file>>();
    qmlRegisterType<ChatSortModel>("org.kde.Tok", 1, 0, "ChatSortModel");
    qmlRegisterType<TokQmlRelationalListener>("org.kde.Tok", 1, 0, "RelationalListener");

    auto c = new Client(testMode);

    eng->rootContext()->setContextObject(new KLocalizedContext(eng));
    eng->rootContext()->setContextProperty("tClient", c);
    eng->addImageProvider("telegram", new TelegramImageProvider(c));
}
