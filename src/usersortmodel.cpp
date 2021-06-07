#include <QDebug>

#include "usersortmodel.h"
#include "userdata.h"

UserSortModel::UserSortModel(QObject* parent) : QSortFilterProxyModel(parent), _store(nullptr)
{
    setDynamicSortFilter(true);
    setFilterRole(Qt::UserRole);
    setSortRole(Qt::UserRole);
    sort(0);
    invalidateFilter();
}

bool UserSortModel::lessThan(const QModelIndex& lhs, const QModelIndex& rhs) const
{
    if (!_store) {
        return true;
    }

    auto lhID = sourceModel()->data(sourceModel()->index(lhs.row(), 0), 0).toString().toLongLong();
    if (!_store->userData.contains(lhID)) {
        return false;
    }

    auto rhID = sourceModel()->data(sourceModel()->index(rhs.row(), 0), 0).toString().toLongLong();
    if (!_store->userData.contains(rhID)) {
        return false;
    }

    return _store->userData[lhID]->username_ < _store->userData[rhID]->username_;
};

bool UserSortModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)

    if (!_store) {
        return true;
    }

    auto id = sourceModel()->data(sourceModel()->index(sourceRow, 0), 0).toString().toLongLong();
    if (!_store->userData.contains(id)) {
        return false;
    }

    if (_store->userData[id]->username_ == "") {
        return false;
    }

    return QString::fromStdString(_store->userData[id]->username_).toLower().startsWith(_filter.toLower());
}

UserDataModel* UserSortModel::store()
{
    return _store;
}

void UserSortModel::setStore(UserDataModel* store)
{
    if (store == _store) {
        return;
    }

    connect(store, &UserDataModel::keyDataChanged, this, [=]() {
        sort(0);
        invalidateFilter();
    });

    _store = store;
    Q_EMIT storeChanged();
    sort(0);
}

QString UserSortModel::filter()
{
    return _filter;
}

void UserSortModel::setFilter(const QString& filter)
{
    if (filter == _filter) {
        return;
    }

    _filter = filter;
    Q_EMIT filterChanged();
    invalidateFilter();
}
