#include "recentstickers_p.h"

enum Roles {
    StickerID,
};

RecentStickersModel::RecentStickersModel(Client* parent) : QAbstractListModel(parent), c(parent), d(new Private)
{
    c->call<TDApi::getRecentStickers>([this](TDApi::getStickers::ReturnType r) {
        beginResetModel();
        d->data = std::move(r->stickers_);
        endResetModel();
    }, false);
}

RecentStickersModel::~RecentStickersModel()
{
}

QVariant RecentStickersModel::data(const QModelIndex& idx, int role) const
{
    auto row = idx.row();
    if (row >= d->data.size()) {
        return QVariant();
    }

    switch (role) {
    case Roles::StickerID:
        return QString::number(d->data[row]->sticker_->id_);
    }

    return QVariant();
}

int RecentStickersModel::rowCount(const QModelIndex& parent) const
{
    return d->data.size();
}

QHash<int,QByteArray> RecentStickersModel::roleNames() const
{
    return {
        { StickerID, "stickerID" }
    };
}


void RecentStickersModel::send(int idx, const QString& toChat)
{
    using namespace TDApi;

    const auto& data = d->data[idx];

    c->call<sendMessage>(nullptr, toChat.toLongLong(), 0, 0, nullptr, nullptr, make_object<inputMessageSticker>(make_object<inputFileId>(data->sticker_->id_), nullptr, data->width_, data->height_, data->emoji_));
}
