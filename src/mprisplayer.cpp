#include <QAudio>
#include <QMediaPlayer>
#include <QMediaMetaData>

#include "mprisplayer.h"
#include "qmediametadata.h"

/*
 * Implementation of adaptor class PlayerAdaptor
 */

PlayerAdaptor::PlayerAdaptor(QObject *parent, QObject* singleton, QMediaPlayer* player)
    : QDBusAbstractAdaptor(parent), p(player), singleton(singleton)
{
    // constructor
    setAutoRelaySignals(true);
    auto notify = [this]() {
        auto notifydbus = [this](const QString &property, const QVariant &value) {
            QVariantMap properties;
            properties[property] = value;
            const int ifaceIndex = metaObject()->indexOfClassInfo("D-Bus Interface");
            QDBusMessage msg = QDBusMessage::createSignal(QStringLiteral("/org/mpris/MediaPlayer2"), QStringLiteral("org.freedesktop.DBus.Properties"), QStringLiteral("PropertiesChanged"));

            msg << QLatin1String(metaObject()->classInfo(ifaceIndex).value());
            msg << properties;
            msg << QStringList();

            QDBusConnection::sessionBus().send(msg);
        };

        notifydbus("CanControl", canControl());
        notifydbus("CanGoNext", canGoNext());
        notifydbus("CanGoPrevious", canGoPrevious());
        notifydbus("CanPause", canPause());
        notifydbus("CanPlay", canPlay());
        notifydbus("CanSeek", canSeek());
        notifydbus("LoopStatus", loopStatus());
        notifydbus("MaximumRate", maximumRate());
        notifydbus("Metadata", metadata());
        notifydbus("MinimumRate", minimumRate());
        notifydbus("PlaybackStatus", playbackStatus());
        notifydbus("Position", position());
        notifydbus("Rate", rate());
        notifydbus("Shuffle", shuffle());
        notifydbus("Volume", volume());
    };
    connect(p, &QMediaPlayer::mediaStatusChanged, this, notify);
    connect(p, &QMediaPlayer::mediaChanged, this, notify);
    connect(p, &QMediaPlayer::stateChanged, this, notify);
}

PlayerAdaptor::~PlayerAdaptor()
{
    // destructor
}

bool PlayerAdaptor::canControl() const
{
    return true;
}

bool PlayerAdaptor::canGoNext() const
{
    return false; // todo: add playlist
}

bool PlayerAdaptor::canGoPrevious() const
{
    return false; // todo: add playlist
}

bool PlayerAdaptor::canPause() const
{
    return p->mediaStatus() != QMediaPlayer::NoMedia;
}

bool PlayerAdaptor::canPlay() const
{
    return p->mediaStatus() != QMediaPlayer::NoMedia;
}

bool PlayerAdaptor::canSeek() const
{
    return p->mediaStatus() != QMediaPlayer::NoMedia;
}

QString PlayerAdaptor::loopStatus() const
{
    return "None";
}

void PlayerAdaptor::setLoopStatus(const QString &value)
{
    Q_UNUSED(value)
}

double PlayerAdaptor::maximumRate() const
{
    return 1.0;
}

QVariantMap PlayerAdaptor::metadata() const
{
    auto result = QVariantMap();

    if (p->mediaStatus() == QMediaPlayer::NoMedia) {
        return {};
    }

    auto removeinvalid = [&result](const QString& it) {
        if (!result[it].isValid()) result.remove(it);
    };
    auto removeinvalidmany = [removeinvalid](const QStringList& keys) {
        for (const auto& it : keys) {
            removeinvalid(it);
        }
    };

    result[QStringLiteral("mpris:length")] = p->duration()*1000;
    result[QStringLiteral("xesam:title")] = p->metaData(QMediaMetaData::Title);
    result[QStringLiteral("xesam:album")] = p->metaData(QMediaMetaData::AlbumTitle);
    result[QStringLiteral("xesam:artist")] = p->metaData(QMediaMetaData::AlbumArtist);
    result[QStringLiteral("xesam:url")] = p->media().request().url().toString();
    result[QStringLiteral("mpris:artUrl")] = singleton->property("thumbnail");

    removeinvalidmany({"mpris:length", "xesam:title", "xesam:album", "xesam:artist", "xesam:url"});

    return result;
}

double PlayerAdaptor::minimumRate() const
{
    return 1.0;
}

QString PlayerAdaptor::playbackStatus() const
{
    switch (p->state()) {
    case QMediaPlayer::PausedState:
        return "Paused";
    case QMediaPlayer::PlayingState:
        return "Playing";
    case QMediaPlayer::StoppedState:
        return "Stopped";
    }
}

qlonglong PlayerAdaptor::position() const
{
    return p->position()*1000;
}

double PlayerAdaptor::rate() const
{
    return 1.0;
}

void PlayerAdaptor::setRate(double value)
{
    Q_UNUSED(value)
}

bool PlayerAdaptor::shuffle() const
{
    return false;
}

void PlayerAdaptor::setShuffle(bool value)
{
    Q_UNUSED(value)
}

double PlayerAdaptor::volume() const
{
    return 1.0;
}

void PlayerAdaptor::setVolume(double value)
{
    Q_UNUSED(value)
}

void PlayerAdaptor::Next()
{

}

void PlayerAdaptor::OpenUri(const QString &Uri)
{
    Q_UNUSED(Uri)
}

void PlayerAdaptor::Pause()
{
    p->pause();
}

void PlayerAdaptor::Play()
{
    p->play();
}

void PlayerAdaptor::PlayPause()
{
    if (p->state() == QMediaPlayer::PausedState) {
        p->play();
    } else {
        p->pause();
    }
}

void PlayerAdaptor::Previous()
{
}

void PlayerAdaptor::Seek(qlonglong Offset)
{
    p->setPosition(p->position() + Offset/1000);
}

void PlayerAdaptor::SetPosition(const QDBusObjectPath &TrackId, qlonglong Position)
{
    Q_UNUSED(TrackId);

    p->setPosition(Position/1000);
}

void PlayerAdaptor::Stop()
{
    p->stop();
}

