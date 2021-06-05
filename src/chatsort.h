// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QSortFilterProxyModel>

class ChatsStore;

class ChatSortModel : public QSortFilterProxyModel
{

    Q_OBJECT

    QString _filter;
    ChatsStore* _store = nullptr;
    Q_PROPERTY(ChatsStore* store READ store WRITE setStore NOTIFY storeChanged)
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)

protected:
    bool lessThan(const QModelIndex& lhs, const QModelIndex& rhs) const override;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

public:
    ChatSortModel(QObject* parent = nullptr);

    ChatsStore* store();
    void setStore(ChatsStore* store);
    Q_SIGNAL void storeChanged();

    QString filter();
    void setFilter(const QString& filter);
    Q_SIGNAL void filterChanged();

};
