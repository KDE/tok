#pragma once

#include <QAbstractListModel>

#include "client.h"

class SearchMessagesModel : public QAbstractListModel
{
    Q_OBJECT

    Client* c;

    struct Private;
    std::unique_ptr<Private> d;

public:
    explicit SearchMessagesModel(Client* parent, TDApi::int53 chatID, std::string chatQuery, std::function<TDApi::object_ptr<TDApi::SearchMessagesFilter>(void)> filter, std::function<TDApi::object_ptr<TDApi::MessageSender>(void)> sender = [](){ return nullptr; });
    ~SearchMessagesModel();

    bool canFetchMore(const QModelIndex& parent) const override;
    void fetchMore(const QModelIndex& parent) override;

    QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QHash<int,QByteArray> roleNames() const override;

};
