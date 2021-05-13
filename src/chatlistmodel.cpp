#include <KLocalizedString>
#include "chatlistmodel_p.h"

enum Roles
{
    Name,
    ID,
};

ChatListModel::ChatListModel(Client* parent) : QAbstractListModel(parent), c(parent), d(new Private)
{
}

ChatListModel::~ChatListModel()
{
}

void ChatListModel::handleUpdate(TDApi::array<TDApi::object_ptr<TDApi::chatFilterInfo>>&& filters)
{
    beginResetModel();
    d->chats.clear();
    d->chats.push_back(TDApi::make_object<TDApi::chatFilterInfo>(TDApi::chatListMain::ID, i18n("All Chats").toStdString(), ""));
    for (auto& it : filters) {
        d->chats.push_back(std::move(it));
    }
    endResetModel();
}

QVariant ChatListModel::data(const QModelIndex& idx, int role) const
{
    if (idx.row() >= int(d->chats.size())) {
        return QVariant();
    }

    auto row = idx.row();

    switch (role) {
    case Roles::Name:
        return QString::fromStdString(d->chats[row]->title_);
    case Roles::ID:
        return QString::number(d->chats[row]->id_);
    }

    return QVariant();
}

int ChatListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return d->chats.size();
}

QHash<int,QByteArray> ChatListModel::roleNames() const
{
    return {
        { Roles::Name, "name" },
        { Roles::ID,   "chatListID" },
    };
}
