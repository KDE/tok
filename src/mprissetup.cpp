#include "mprissetup.h"
#include "mpris.h"
#include "mprisplayer.h"

void setupMPRIS(QObject* app, QObject* singleton, QMediaPlayer* player)
{
    QString mpris2Name = QStringLiteral("org.mpris.MediaPlayer2.org.kde.Tok");
    bool canRegister = QDBusConnection::sessionBus().registerService(mpris2Name);

    if (!canRegister) {
        canRegister = QDBusConnection::sessionBus().registerService(QLatin1String("org.mpris.MediaPlayer2.org.kde.Tok.instance") % QString::number(qApp->applicationPid()));
    }

    new MediaPlayer2Adaptor(app);
    new PlayerAdaptor(app, singleton, player);
    QDBusConnection::sessionBus().registerObject(QLatin1String("/org/mpris/MediaPlayer2"), app, QDBusConnection::ExportAdaptors);
}
