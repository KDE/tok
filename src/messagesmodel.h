#pragma once

#include <QAbstractListModel>

#include "client.h"

class MessagesModel : public QAbstractListModel
{
    Q_OBJECT

    Client* c;

    struct Private;
    std::unique_ptr<Private> d;

public:
    explicit MessagesModel(Client* parent, TDApi::int53 id);
    ~MessagesModel();

    void fetch();
    // void handleUpdate(TDApi::object_ptr<TDApi::Update> u);

    // bool canFetchMore(const QModelIndex& parent) const override;
    // void fetchMore(const QModelIndex& parent) override;

    QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QHash<int,QByteArray> roleNames() const override;
};
