#include <QQmlComponent>

#include "instantviewer.h"

void InstantViewer::doInstantView(QJSValue callback, const QString &url, Client *c, QQuickItem* item)
{
    using namespace TDApi;

    c->call<getWebPageInstantView>(
        [cb = callback, item](getWebPageInstantView::ReturnType ret) {
            auto it = cb;

            QQmlComponent label(qmlEngine(item));
            label.setData("import QtQuick 2.12; import QtQuick.Controls 2.12; Label { wrapMode: Text.Wrap; }", QUrl("instantviewer.cpp"));
            while (!label.isReady()) {
                QCoreApplication::processEvents();
            }
            auto make = [&label](const QString& txt) { auto it = label.create(); it->setProperty("text", txt); return it; };

            for (auto& item : ret->page_blocks_) {
            match (item)
                handleCase(pageBlockTitle, title)
                endhandle
            endmatch
            }
        },
        url.toStdString(), true
    );
}
