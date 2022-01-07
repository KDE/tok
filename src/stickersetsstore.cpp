#include "stickersetsstore_p.h"
#include "photoutils.h"

enum RoleNames {
    Title,
    Name,
    Stickers,
    Thumbnail,
};

StickerSetsStore::StickerSetsStore(Client* parent) : TokAbstractRelationalModel(parent), c(parent), d(new Private)
{

}
StickerSetsStore::~StickerSetsStore()
{

}

Sticker::Sticker()
{

}
Sticker::~Sticker()
{

}

Sticker Sticker::fromTDApi(const TDApi::object_ptr<TDApi::sticker>& sticker)
{
    Sticker ret;

    ret.width = sticker->width_;
    ret.height = sticker->height_;
    ret.emoji = QString::fromStdString(sticker->emoji_);
    ret.isAnimated = sticker->is_animated_;
    if (sticker->thumbnail_ != nullptr) {
        ret.thumbnail = imageToURL(sticker->thumbnail_->file_);
    }
    ret.stickerURL = QString::number(sticker->sticker_->id_);

    return ret;
}

void StickerSetsStore::updateSet(TDApi::object_ptr<TDApi::stickerSet>& data)
{
    QVariantList stickers;
    stickers.reserve(data->stickers_.size());
    for (const auto& sticker : data->stickers_) {
        stickers << QVariant::fromValue(Sticker::fromTDApi(sticker));
    }

    const auto id = data->id_;

    d->stickerData[id] = stickers;
    d->stickerSetData[id] = std::move(data);

    Q_EMIT keyAdded(QString::number(id));
}

void StickerSetsStore::handleUpdate(TDApi::object_ptr<TDApi::updateStickerSet> c)
{
    updateSet(c->sticker_set_);
}

QVariant StickerSetsStore::data(const QVariant& key, int role)
{
    if (!checkKey(key))
        return QVariant();

    const auto id = key.toString().toULongLong();
    const auto& data = d->stickerSetData[id];

    switch (RoleNames(role)) {
    case Title:
        return QString::fromStdString(data->title_);
    case Name:
        return QString::fromStdString(data->name_);
    case Stickers:
        return d->stickerData[id];
    case Thumbnail:
        if (data->thumbnail_ == nullptr)
            return QVariant();

        return imageToURL(data->thumbnail_->file_);
    }

    return QVariant();
}
bool StickerSetsStore::checkKey(const QVariant& key)
{
    return d->stickerSetData.contains(key.toString().toULongLong());
}
bool StickerSetsStore::canFetchKey(const QVariant& key)
{
    bool ok = false;
    key.toString().toULongLong(&ok);
    return ok;
}
void StickerSetsStore::fetchKey(const QVariant& key)
{
    c->call<TDApi::getStickerSet>([this](TDApi::object_ptr<TDApi::stickerSet> resp) {
        updateSet(resp);
    }, key.toString().toULongLong());
}
QHash<int, QByteArray> StickerSetsStore::roleNames()
{
    return {
        { Title, "title" },
        { Name, "name" },
        { Stickers, "stickers" },
        { Thumbnail, "thumbnail" },
    };
}