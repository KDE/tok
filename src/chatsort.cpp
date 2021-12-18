// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QDebug>
#include <QTimer>

#include "chatsort.h"
#include "chatsstore_p.h"

ChatSortModel::ChatSortModel(QObject* parent) : QSortFilterProxyModel(parent), _store(nullptr), _folder(QString::number(TDApi::chatListMain::ID))
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

    const auto folderID = _folder.toLongLong();
    const auto isFolder = folderID != TDApi::chatListMain::ID;

    auto& data = _store->d->chatData[id];
    for (const auto& pos : data->positions_) {
        if (!isFolder && pos->list_->get_id() == TDApi::chatListMain::ID) {
            goto ok;
        }
        if (pos->list_->get_id() == TDApi::chatListFilter::ID && static_cast<const TDApi::chatListFilter*>(pos->list_.get())->chat_filter_id_ == folderID) {
            goto ok;
        }
    }
    return false;

ok:

    if (_filter.isEmpty()) {
        return true;
    }

    const auto title = _store->data(QString::number(id), Qt::UserRole).toString();

    return title.toLower().contains(_filter.toLower());
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
        setDynamicSortFilter(false);
        sort(0);
        invalidateFilter();
        setDynamicSortFilter(true);
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

QString ChatSortModel::folder()
{
    return _folder;
}

void ChatSortModel::setFolder(const QString& folder)
{
    if (folder == _folder) {
        return;
    }

    auto it = folder.toLongLong();

    TDApi::object_ptr<TDApi::ChatList> list = nullptr;
    if (it != TDApi::chatListMain::ID) {
        list = TDApi::make_object<TDApi::chatListFilter>(it);
    }

    _store->c->call<TDApi::loadChats>(
        nullptr,
        std::move(list), 20
    );

    _folder = folder;
    Q_EMIT folderChanged();
    invalidateFilter();
}
