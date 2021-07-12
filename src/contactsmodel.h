// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QAbstractListModel>

#include "client.h"

class ContactsModel : public QAbstractListModel
{
    Q_OBJECT

    Client* c;

    struct Private;
    std::unique_ptr<Private> d;

    Q_PROPERTY(QStringList selectedIDs READ selectedIDs NOTIFY selectedIDsChanged)

public:
    explicit ContactsModel(Client* parent);
    ~ContactsModel();

    QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QStringList selectedIDs() const;
    Q_SIGNAL void selectedIDsChanged();

    Q_INVOKABLE void select(const QString& it);

    QHash<int,QByteArray> roleNames() const override;
};
