#include <QGuiApplication>
#include <QTextBoundaryFinder>

#include <unicode/urename.h>
#include <unicode/uchar.h>

#include "utilities.h"

QString Utilities::emojified(const QString &in)
{
    auto cp = in.toHtmlEscaped();
    QString out;

    QTextBoundaryFinder finder(QTextBoundaryFinder::Grapheme, cp);
    int pos = 0;
    while (finder.toNextBoundary() != -1) {
        auto range = finder.position();

        auto it = cp.mid(pos, range-pos);

        auto first = it.toUcs4()[0];

        if (u_hasBinaryProperty(first, UCHAR_EMOJI_PRESENTATION)) {
            out.append(QString(R"(<font size="+1" face="emoji, sans-serif">%1</font>)").arg(it));
        } else {
            out.append(it);
        }

        pos = range;
    }

    return out;
}
