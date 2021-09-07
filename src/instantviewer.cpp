#include <QQmlComponent>
#include <QQuickTextDocument>
#include <QTextCursor>
#include <QTextDocument>

#include "instantviewer.h"

inline auto component(QQuickItem* item, const QString& cont)
{
    QSharedPointer<QQmlComponent> label(new QQmlComponent(qmlEngine(item)));
    label->setData(cont.toLocal8Bit(), QUrl());
    while (!label->isReady()) {
        if (label->isError()) {
            qWarning() << label->errorString();
            qFatal("");
        }
        QCoreApplication::processEvents();
    }

    return label;
}

inline void makeRich(QTextDocument* document, TDApi::object_ptr<TDApi::RichText>& cont)
{
    using namespace TDApi;

    QTextCursor curs(document);

    match (cont)
        handleCase(richTextPlain, text)
            curs.insertText(QString::fromStdString(text->text_));
        endhandle
        handleCase(richTexts, text)
            for (auto& item : text->texts_) {
                makeRich(document, item);
            }
        endhandle
        default: {
            curs.insertText("unsupported");
        }
    endmatch
}

void InstantViewer::doInstantView(const QString &url, Client *c, QQuickItem* item)
{
    using namespace TDApi;

    c->call<getWebPageInstantView>(
        [item](getWebPageInstantView::ReturnType ret) {
            qWarning() << "doing...";

            auto textArea = component(item, R"(
import QtQuick 2.15
import QtQuick.Layouts 1.12
import org.kde.kirigami 2.15 as Kirigami

TextEdit {
    readOnly: true
    selectByMouse: !Kirigami.Settings.isMobile
    wrapMode: Text.Wrap

    color: Kirigami.Theme.textColor
    selectedTextColor: Kirigami.Theme.highlightedTextColor
    selectionColor: Kirigami.Theme.highlightColor

    Layout.fillWidth: true
}
            )");
            auto column = component(item, R"(
import QtQuick.Layouts 1.12

ColumnLayout {
    Layout.fillWidth: true
}
            )");

            auto citem = qobject_cast<QQuickItem*>(column->create());
            citem->setParentItem(item);
            QQuickItem* text;
            QTextDocument* doku;

            auto step = [&text, &doku, &textArea, citem]() {
                text = qobject_cast<QQuickItem*>(textArea->create(qmlContext(citem)));
                text->setParentItem(citem);
                doku = text->property("textDocument").value<QQuickTextDocument*>()->textDocument();
            };

            qWarning() << "making...";

            for (auto& item : ret->page_blocks_) {
            match (item)
                handleCase(pageBlockTitle, title)
                    step();
                    makeRich(doku, title->title_);
                endhandle
                handleCase(pageBlockParagraph, para)
                    step();
                    makeRich(doku, para->text_);
                endhandle
            endmatch
            }
        },
        url.toStdString(), true
    );
}
