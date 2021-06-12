#pragma once

#include <QAbstractListModel>

#include "client.h"

class RecentStickersModel : public QAbstractListModel
{
    Q_OBJECT

    Client* c;

    struct Private;
    std::unique_ptr<Private> d;

public:
    explicit RecentStickersModel(Client* parent);
    ~RecentStickersModel();

    QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QHash<int,QByteArray> roleNames() const override;

    Q_INVOKABLE void send(int idx, const QString& toChat);
};
