#include <QQmlComponent>
#include <QQuickTextDocument>
#include <QTextCursor>
#include <QTextDocument>

#include "instantviewer.h"

using namespace TDApi;

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

#define op object_ptr
#define pb PageBlock
#define qqi QQuickItem
#define B bool
#define t TDApi::
#define c Context
#define db(x) x; break;
#define sc static_cast
#define I(c,t,f) c ? t : f
#define ob(k) sc<k*>(blk.get())
#define dbs(k, f) db(txt = ob(k)->f.get());
#define cdbs(k, f) case k::ID: dbs(k, f)
#define q(k) case k::ID: db(txt = I(iC, ob(k)->credit_.get(), ob(k)->text_.get()));
#define p(i) pageBlock ## i
#define rt RichText
#define N nullptr

struct Context
{
    QSharedPointer<QQmlComponent> ctx;

    Context(QQuickItem* item) {
        ctx = component(item, R"()");
    }

    qqi* pTxt(op<pb>& blk, B isCaption);
    qqi* pUsp(op<pb>& blk);
    qqi* pBlk(op<pb>& blk);
};


qqi* c::pTxt(op<pb>& blk, B iC)
{
    rt* txt = N;

    switch (blk->get_id()) {
    cdbs(p(Title), title_) cdbs(p(Subtitle), subtitle_)
    cdbs(p(Header), header_) cdbs(p(Subheader), subheader_)
    cdbs(p(Footer), footer_) cdbs(p(Paragraph), text_)
    cdbs(p(Preformatted), text_) cdbs(p(Details), header_)
    cdbs(p(Table), caption_) cdbs(p(RelatedArticles), header_)
    cdbs(p(Kicker), kicker_) q(p(BlockQuote))
    q(p(PullQuote))
    }
}

QQuickItem* c::pUsp(op<pb>& blk)
{

}

QQuickItem* c::pBlk(op<pb>& blk)
{
    switch (blk->get_id()) {
    case pageBlockHeader::ID:
    case pageBlockSubheader::ID:
    case pageBlockTitle::ID:
    case pageBlockSubtitle::ID:
    case pageBlockFooter::ID:
    case pageBlockParagraph::ID:
    case pageBlockKicker::ID:
        return pTxt(blk, false);
    case pageBlockCover::ID:
        // return processCover(cover);
    case pageBlockAuthorDate::ID:
        // return processAuthorDate(authorDate);
    case pageBlockBlockQuote::ID:
        // return processBlockquote(blockquote);
    case pageBlockDivider::ID:
        // return processDivider(divider);
    case pageBlockPhoto::ID:
        // return processPhoto(photo);
    case pageBlockList::ID:
        // return processList(list);
    case pageBlockVideo::ID:
        // return processVideo(video);
    case pageBlockAnimation::ID:
        // return processAnimation(animation);
    case pageBlockEmbeddedPost::ID:
        // return processEmbedPost(embedPost);
    case pageBlockSlideshow::ID:
        // return processSlideshow(slideshow);
    case pageBlockCollage::ID:
        // return processCollage(collage);
    case pageBlockEmbedded::ID:
        // return processEmbed(embed);
    case pageBlockPullQuote::ID:
        // return processPullquote(pullquote);
    case pageBlockAnchor::ID:
        // return processAnchor(anchor);
    case pageBlockPreformatted::ID:
        // return processPreformatted(preformatted);
    case pageBlockChatLink::ID:
        // return processChannel(channel);
    case pageBlockDetails::ID:
        // return processDetails(details);
    case pageBlockTable::ID:
        // return processTable(table);
    case pageBlockRelatedArticles::ID:
        // return processRelatedArticles(relatedArticles);
    case pageBlockMap::ID:
        // return processMap(map);
    default:
        return pUsp(blk);
    }
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
