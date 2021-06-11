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
