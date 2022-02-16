#pragma once

#include <QAbstractListModel>

#include "client.h"

class BackgroundsModel : public QAbstractListModel
{
    Q_OBJECT

    Client* c;

    struct Private;
    std::unique_ptr<Private> d;

public:
    explicit BackgroundsModel(Client* parent);
    ~BackgroundsModel();

    QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QHash<int,QByteArray> roleNames() const override;
};
