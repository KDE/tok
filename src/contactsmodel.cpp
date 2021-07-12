#include "contactsmodel_p.h"

enum Roles {
    ID,
    Selected,
};

ContactsModel::ContactsModel(Client* parent) : QAbstractListModel(parent), c(parent), d(new Private)
{
    d->data = TDApi::make_object<TDApi::users>();

    c->call<TDApi::getContacts>(
        [this](TDApi::getContacts::ReturnType ret) {
            beginResetModel();
            d->data = std::move(ret);
            endResetModel();
        }
    );
}

ContactsModel::~ContactsModel()
{

}

QVariant ContactsModel::data(const QModelIndex& idx, int role) const
{
    auto row = idx.row();
    if (row >= d->data->user_ids_.size()) {
        return QVariant();;
    }

    auto id = d->data->user_ids_[row];

    switch (Roles(role)) {
    case ID:
        return QString::number(id);
    case Selected:
        return d->selectedIDs.contains(id);
    }

    return QVariant();
}

void ContactsModel::select(const QString& it)
{
    auto id = it.toLongLong();
    auto idx = -1;
    for (auto i : d->data->user_ids_) {
        idx++;
        if (i == id) {
            break;
        }
    }
    if (d->selectedIDs.contains(id)) {
        d->selectedIDs.remove(id);
    } else {
        d->selectedIDs.insert(id);
    }
    Q_EMIT selectedIDsChanged();
    Q_EMIT dataChanged(index(idx), index(idx), {Roles::Selected});
}

QStringList ContactsModel::selectedIDs() const
{
    QStringList ret;
    ret.reserve(d->selectedIDs.size());
    for (auto item : d->selectedIDs) {
        ret << QString::number(item);
    }
    return ret;
}

int ContactsModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return d->data->user_ids_.size();
}

QHash<int,QByteArray> ContactsModel::roleNames() const
{
    return {
        { ID, "userID" },
        { Selected, "isSelected" },
    };
}

