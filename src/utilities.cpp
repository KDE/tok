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
    KWindowEffects::enableBackgroundContrast(item->window(), doit);
    KWindowEffects::enableBlurBehind(item->window(), doit);
    item->window();
}
