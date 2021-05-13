#pragma once

#include <QAbstractListModel>

#include "client.h"

class ChatListModel : public QAbstractListModel
{
    Q_OBJECT

    Client* c;

    struct Private;
    std::unique_ptr<Private> d;

public:
    explicit ChatListModel(Client* parent);
    ~ChatListModel();

    void handleUpdate(TDApi::array<TDApi::object_ptr<TDApi::chatFilterInfo>>&& filters);

    QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QHash<int,QByteArray> roleNames() const override;
};
