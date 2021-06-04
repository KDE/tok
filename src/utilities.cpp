#include <QGuiApplication>
#include <QQuickWindow>
#include <KWindowEffects>

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
