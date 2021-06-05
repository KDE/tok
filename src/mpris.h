// Copyright (C) 2020 The Qt Company Ltd.
// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QMetaObject>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Adaptor class for interface org.mpris.MediaPlayer2
 */
class MediaPlayer2Adaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2")
	Q_CLASSINFO("D-Bus Introspection",
				""
				"  <interface name=\"org.mpris.MediaPlayer2\">\n"
				"	<annotation value=\"true\" "
				"name=\"org.freedesktop.DBus.Property.EmitsChangedSignal\"/>\n"
				"	<method name=\"Raise\">\n"
				"	</method>\n"
				"	<method name=\"Quit\">\n"
				"	</method>\n"
				"	<property access=\"read\" type=\"b\" name=\"CanQuit\">\n"
				"	</property>\n"
				"	<property access=\"readwrite\" type=\"b\" "
				"name=\"Fullscreen\">\n"
				"	  <annotation value=\"true\" "
				"name=\"org.mpris.MediaPlayer2.property.optional\"/>\n"
				"	</property>\n"
				"	<property access=\"read\" type=\"b\" "
				"name=\"CanSetFullscreen\">\n"
				"	  <annotation value=\"true\" "
				"name=\"org.mpris.MediaPlayer2.property.optional\"/>\n"
				"	</property>\n"
				"	<property access=\"read\" type=\"b\" name=\"CanRaise\">\n"
				"	</property>\n"
				"	<property access=\"read\" type=\"b\" name=\"HasTrackList\">\n"
				"	</property>\n"
				"	<property access=\"read\" type=\"s\" name=\"Identity\">\n"
				"	</property>\n"
				"	<property access=\"read\" type=\"s\" name=\"DesktopEntry\">\n"
				"	  <annotation value=\"true\" "
				"name=\"org.mpris.MediaPlayer2.property.optional\"/>\n"
				"	</property>\n"
				"	<property access=\"read\" type=\"as\" "
				"name=\"SupportedUriSchemes\">\n"
				"	</property>\n"
				"	<property access=\"read\" type=\"as\" "
				"name=\"SupportedMimeTypes\">\n"
				"	</property>\n"
				"  </interface>\n"
				"")
public:
	MediaPlayer2Adaptor(QObject *parent);
	virtual ~MediaPlayer2Adaptor();

public: // PROPERTIES
	Q_PROPERTY(bool CanQuit READ canQuit)
	bool canQuit() const;

	Q_PROPERTY(bool CanRaise READ canRaise)
	bool canRaise() const;

	Q_PROPERTY(bool CanSetFullscreen READ canSetFullscreen)
	bool canSetFullscreen() const;

	Q_PROPERTY(QString DesktopEntry READ desktopEntry)
	QString desktopEntry() const;

	Q_PROPERTY(bool Fullscreen READ fullscreen WRITE setFullscreen)
	bool fullscreen() const;
	void setFullscreen(bool value);

	Q_PROPERTY(bool HasTrackList READ hasTrackList)
	bool hasTrackList() const;

	Q_PROPERTY(QString Identity READ identity)
	QString identity() const;

	Q_PROPERTY(QStringList SupportedMimeTypes READ supportedMimeTypes)
	QStringList supportedMimeTypes() const;

	Q_PROPERTY(QStringList SupportedUriSchemes READ supportedUriSchemes)
	QStringList supportedUriSchemes() const;

public Q_SLOTS:
	void Quit();
	void Raise();
};
