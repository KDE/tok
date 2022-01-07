#include "stickersetsmodel_p.h"

enum RoleNames {
    ID,
};

StickerSetsModel::StickerSetsModel(Client* parent) : QAbstractListModel(parent), c(parent), d(new Private)
{

}

StickerSetsModel::~StickerSetsModel()
{

}

void StickerSetsModel::handleUpdate(TDApi::object_ptr<TDApi::updateInstalledStickerSets> u)
{
    if (u->is_masks_)
        return;

    beginResetModel();
    d->stickerIDs = u->sticker_set_ids_;
    endResetModel();
}

QVariant StickerSetsModel::data(const QModelIndex& idx, int role) const
{
    const auto row = idx.row();
    if (row >= d->stickerIDs.size()) {
        return QVariant();
    }

    switch (RoleNames(role)) {
    case RoleNames::ID:
        return QString::number(d->stickerIDs[row]);
    }

    return QVariant();
}

int StickerSetsModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return d->stickerIDs.size();
}

QHash<int,QByteArray> StickerSetsModel::roleNames() const
{
    return {
        { RoleNames::ID, "packID" }
    };
}
