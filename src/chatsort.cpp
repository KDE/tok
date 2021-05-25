#include <QDebug>

#include "chatsort.h"
#include "chatsstore_p.h"

ChatSortModel::ChatSortModel(QObject* parent) : QSortFilterProxyModel(parent), _store(nullptr)
{
    setDynamicSortFilter(true);
    setFilterRole(Qt::UserRole);
    setSortRole(Qt::UserRole);
    sort(0);
    invalidateFilter();
}

bool ChatSortModel::lessThan(const QModelIndex& lhs, const QModelIndex& rhs) const
{
    if (!_store) {
        return true;
    }

    auto locate = [=](TDApi::int53 chatID) {
        auto& data = _store->d->chatData[chatID];
        for (const auto& pos : data->positions_) {
            if (pos->list_->get_id() == TDApi::chatListMain::ID) {
                return pos->order_;
            }
        }
        return -1L;
    };

    auto lhID = sourceModel()->data(lhs, Qt::UserRole).toString().toLongLong();
    if (!_store->d->chatData.contains(lhID)) {
        return true;
    }
    auto lhOrd = locate(lhID);
    if (lhOrd == -1L) {
        return true;
    }

    auto rhID = sourceModel()->data(rhs, Qt::UserRole).toString().toLongLong();
    if (!_store->d->chatData.contains(lhID)) {
        return true;
    }
    auto rhOrd = locate(rhID);
    if (rhOrd == -1L) {
        return true;
    }

    return qMakePair(lhOrd, lhID) > qMakePair(rhOrd, rhID);
};

bool ChatSortModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (!_store) {
        return true;
    }

    auto id = sourceModel()->data(sourceModel()->index(sourceRow, 0), Qt::UserRole).toString().toLongLong();

    if (!_store->d->chatData.contains(id)) {
        return false;
    }

    auto& data = _store->d->chatData[id];
    for (const auto& pos : data->positions_) {
        if (pos->list_->get_id() == TDApi::chatListMain::ID) {
            goto ok;
        }
    }
    return false;

ok:

    if (_filter.isEmpty()) {
        return true;
    }

    return QString::fromStdString(_store->d->chatData[id]->title_).toLower().contains(_filter.toLower());
}

ChatsStore* ChatSortModel::store()
{
    return _store;
}

void ChatSortModel::setStore(ChatsStore* store)
{
    if (store == _store) {
        return;
    }

    connect(store, &ChatsStore::keyDataChanged, this, [=]() {
        sort(0);
        invalidateFilter();
    });

    _store = store;
    Q_EMIT storeChanged();
    sort(0);
}

QString ChatSortModel::filter()
{
    return _filter;
}

void ChatSortModel::setFilter(const QString& filter)
{
    if (filter == _filter) {
        return;
    }

    _filter = filter;
    Q_EMIT filterChanged();
    invalidateFilter();
}
