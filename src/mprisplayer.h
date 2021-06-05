// Copyright © 2006-2012 the VideoLAN team (Mirsal Ennaime, Rafaël Carré, Jean-Paul Saman)
// Copyright © 2005-2008 Milosz Derezynski
// Copyright © 2008 Nick Welch
// Copyright © 2010-2012 Alex Merry
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef MPRISPLAYER_CPP
#define MPRISPLAYER_CPP

#include <QtCore/QObject>
#include <QtCore/QMetaObject>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

class QMediaPlayer;

/*
 * Adaptor class for interface org.mpris.MediaPlayer2.Player
 */
class PlayerAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Player")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"org.mpris.MediaPlayer2.Player\">\n"
"    <method name=\"Next\"/>\n"
"    <method name=\"Previous\"/>\n"
"    <method name=\"Pause\"/>\n"
"    <method name=\"PlayPause\"/>\n"
"    <method name=\"Stop\"/>\n"
"    <method name=\"Play\"/>\n"
"    <method name=\"Seek\">\n"
"      <arg direction=\"in\" type=\"x\" name=\"Offset\"/>\n"
"    </method>\n"
"    <method name=\"SetPosition\">\n"
"      <arg direction=\"in\" type=\"o\" name=\"TrackId\"/>\n"
"      <arg direction=\"in\" type=\"x\" name=\"Position\"/>\n"
"    </method>\n"
"    <method name=\"OpenUri\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"Uri\"/>\n"
"    </method>\n"
"    <signal name=\"Seeked\">\n"
"      <arg type=\"x\" name=\"Position\"/>\n"
"    </signal>\n"
"    <property access=\"read\" type=\"s\" name=\"PlaybackStatus\"/>\n"
"    <property access=\"readwrite\" type=\"s\" name=\"LoopStatus\"/>\n"
"    <property access=\"readwrite\" type=\"d\" name=\"Rate\"/>\n"
"    <property access=\"readwrite\" type=\"b\" name=\"Shuffle\"/>\n"
"    <property access=\"read\" type=\"a{sv}\" name=\"Metadata\">\n"
"      <annotation value=\"QVariantMap\" name=\"org.qtproject.QtDBus.QtTypeName\"/>\n"
"    </property>\n"
"    <property access=\"readwrite\" type=\"d\" name=\"Volume\"/>\n"
"    <property access=\"read\" type=\"x\" name=\"Position\"/>\n"
"    <property access=\"read\" type=\"d\" name=\"MinimumRate\"/>\n"
"    <property access=\"read\" type=\"d\" name=\"MaximumRate\"/>\n"
"    <property access=\"read\" type=\"b\" name=\"CanGoNext\"/>\n"
"    <property access=\"read\" type=\"b\" name=\"CanGoPrevious\"/>\n"
"    <property access=\"read\" type=\"b\" name=\"CanPlay\"/>\n"
"    <property access=\"read\" type=\"b\" name=\"CanPause\"/>\n"
"    <property access=\"read\" type=\"b\" name=\"CanSeek\"/>\n"
"    <property access=\"read\" type=\"b\" name=\"CanControl\"/>\n"
"  </interface>\n"
        "")

    QMediaPlayer* p;
    QObject* singleton;

public:
    PlayerAdaptor(QObject *parent, QObject* singleton, QMediaPlayer* player);
    virtual ~PlayerAdaptor();

public: // PROPERTIES
    Q_PROPERTY(bool CanControl READ canControl)
    bool canControl() const;

    Q_PROPERTY(bool CanGoNext READ canGoNext)
    bool canGoNext() const;

    Q_PROPERTY(bool CanGoPrevious READ canGoPrevious)
    bool canGoPrevious() const;

    Q_PROPERTY(bool CanPause READ canPause)
    bool canPause() const;

    Q_PROPERTY(bool CanPlay READ canPlay)
    bool canPlay() const;

    Q_PROPERTY(bool CanSeek READ canSeek)
    bool canSeek() const;

    Q_PROPERTY(QString LoopStatus READ loopStatus WRITE setLoopStatus)
    QString loopStatus() const;
    void setLoopStatus(const QString &value);

    Q_PROPERTY(double MaximumRate READ maximumRate)
    double maximumRate() const;

    Q_PROPERTY(QVariantMap Metadata READ metadata)
    QVariantMap metadata() const;

    Q_PROPERTY(double MinimumRate READ minimumRate)
    double minimumRate() const;

    Q_PROPERTY(QString PlaybackStatus READ playbackStatus)
    QString playbackStatus() const;

    Q_PROPERTY(qlonglong Position READ position)
    qlonglong position() const;

    Q_PROPERTY(double Rate READ rate WRITE setRate)
    double rate() const;
    void setRate(double value);

    Q_PROPERTY(bool Shuffle READ shuffle WRITE setShuffle)
    bool shuffle() const;
    void setShuffle(bool value);

    Q_PROPERTY(double Volume READ volume WRITE setVolume)
    double volume() const;
    void setVolume(double value);

public Q_SLOTS: // METHODS
    void Next();
    void OpenUri(const QString &Uri);
    void Pause();
    void Play();
    void PlayPause();
    void Previous();
    void Seek(qlonglong Offset);
    void SetPosition(const QDBusObjectPath &TrackId, qlonglong Position);
    void Stop();
Q_SIGNALS: // SIGNALS
    void Seeked(qlonglong Position);
};

#endif
