#pragma once

#include <QAbstractListModel>

#include "client.h"

class ChatsModel : public QAbstractListModel
{
    Q_OBJECT

    Client* c;

    struct Private;
    std::unique_ptr<Private> d;

private:
    void updateChat(TDApi::object_ptr<TDApi::chat> c);

public:
    explicit ChatsModel(Client* parent);
    ~ChatsModel();

    void fetch();
    void handleUpdate(TDApi::object_ptr<TDApi::Update> u);

    QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QHash<int,QByteArray> roleNames() const override;
};
