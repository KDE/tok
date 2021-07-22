// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QGuiApplication>
#include <QQuickWindow>
#include <KWindowEffects>
#include <QFileDialog>
#include <QStandardPaths>

#include "utilities.h"

bool Utilities::isRTL(const QString& str)
{
    for (const auto& rune : str) {
        if (rune.direction() == QChar::DirR || rune.direction() == QChar::DirAL) {
            return true;
        } else if (rune.direction() == QChar::DirL) {
            return false;
        }
    }
    return QGuiApplication::isRightToLeft();
}

void Utilities::setBlur(QQuickItem* item, bool doit)
{
    auto setWindows = [=]() {
        static const bool isMaui = !qgetenv("TOK_MAUI").isEmpty();

        auto reg = QRect(QPoint(0, 0), item->window()->size());
        if (isMaui) {
            reg.adjust(2, 2, -2, -2);
        }

        KWindowEffects::enableBackgroundContrast(item->window(), doit, 1, 1, 1, reg);
        KWindowEffects::enableBlurBehind(item->window(), doit, reg);
    };

    connect(item->window(), &QQuickWindow::heightChanged, this, setWindows);
    connect(item->window(), &QQuickWindow::widthChanged, this, setWindows);
    setWindows();
}

QString Utilities::wordAt(int pos, const QString& in)
{
    if (in.length() == 0) return QString();

    int first = 0, last = 0;

    if (pos == in.length()) {
        last = pos-1;
    } else for (int i = pos; i < in.length(); i++) {
        last = i;
        if (in[i] == ' ') {
            last--;
            break;
        }
    }

    if (pos-1 <= 0) {
        first = 0;
    } else for (int i = pos-1 >= in.length() ? in.length()-1 : pos-1; i >= 0; i--) {
        first = i;
        if (in[i] == ' ') {
            first++;
            break;
        }
    }

    return in.mid(first, last-first+1);
}

QIviPendingReplyBase Utilities::pickFile(const QString& title, const QString& standardLocation)
{
    QIviPendingReply<QUrl> it;

    QFileDialog* dia = new QFileDialog;
    dia->setWindowTitle(title);
    dia->setDirectory(QStandardPaths::standardLocations(standardLocation == "photo" ? QStandardPaths::PicturesLocation : QStandardPaths::HomeLocation).last());

    connect(dia, &QFileDialog::accepted, this, [it, dia]() mutable {
        it.setSuccess(QUrl::fromLocalFile(dia->selectedFiles().last()));
    });
    connect(dia, &QFileDialog::rejected, this, [it]() mutable {
        it.setFailed();
    });

    dia->open();

    return it;
}

QString Utilities::humanSize(int size)
{
    return QLocale().formattedDataSize(size, 1);
}

#include <QMimeDatabase>
#include <QMimeType>
#include <QImageReader>

QJsonObject Utilities::fileData(const QString& url)
{
    QFileInfo fi(QUrl(url).toLocalFile());

    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(url);

    QJsonObject obj;
    obj["size"] = fi.size();
    obj["name"] = fi.baseName();
    obj["type"] = typeOfFile(url);
    obj["icon"] = mime.iconName();

    return obj;
}

QString Utilities::fileIcon(const QUrl &url)
{
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(url.toLocalFile());

    return mime.iconName();
}

QString Utilities::typeOfFile(const QUrl& url)
{
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(url.toLocalFile());

    if (mime.inherits("video/mp4")) {
        return "video";
    } else if (mime.inherits("image/jpeg") || mime.inherits("image/png") || mime.inherits("image/png")) {
        QImageReader reader(url.toLocalFile());

        auto size = reader.size();
        auto width = size.width(), height = size.height();
        if (width+height > 10000) {
            goto breakout;
        }
        if (width > height*20 or height > width*20) {
            goto breakout;
        }

        QFile it(url.toLocalFile());
        if (it.size() > 10000000) {
            goto breakout;
        }

        return "image";
    } else if (mime.inherits("audio/mpeg")) {
        return "audio";
    }

breakout:
    return "file";
}
