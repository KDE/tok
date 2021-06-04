#include "mpris.h"

MediaPlayer2Adaptor::MediaPlayer2Adaptor(QObject *parent)
	: QDBusAbstractAdaptor(parent)
{
	// constructor
	setAutoRelaySignals(true);
}

MediaPlayer2Adaptor::~MediaPlayer2Adaptor()
{
	// destructor
}

bool MediaPlayer2Adaptor::canQuit() const
{
	return true;
}

bool MediaPlayer2Adaptor::canRaise() const
{
	return false;
}

bool MediaPlayer2Adaptor::canSetFullscreen() const
{
	return false;
}

QString MediaPlayer2Adaptor::desktopEntry() const
{
	return QStringLiteral("org.kde.Tok");
}

bool MediaPlayer2Adaptor::fullscreen() const
{
	return false;
}

void MediaPlayer2Adaptor::setFullscreen(bool value)
{
	Q_UNUSED(value)
}

bool MediaPlayer2Adaptor::hasTrackList() const
{
	return false;
}

QString MediaPlayer2Adaptor::identity() const
{
	return QStringLiteral("Tok");
}

QStringList MediaPlayer2Adaptor::supportedMimeTypes() const
{
	return QStringList();
}

QStringList MediaPlayer2Adaptor::supportedUriSchemes() const
{
	return QStringList();
}

void MediaPlayer2Adaptor::Quit()
{
	qApp->quit();
}

void MediaPlayer2Adaptor::Raise()
{
}
