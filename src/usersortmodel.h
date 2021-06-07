#pragma once

#include <QSortFilterProxyModel>

class UserDataModel;

class UserSortModel : public QSortFilterProxyModel
{

    Q_OBJECT

    QString _filter;
    UserDataModel* _store = nullptr;
    Q_PROPERTY(UserDataModel* store READ store WRITE setStore NOTIFY storeChanged)
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)

protected:
    bool lessThan(const QModelIndex& lhs, const QModelIndex& rhs) const override;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

public:
    UserSortModel(QObject* parent = nullptr);

    UserDataModel* store();
    void setStore(UserDataModel* store);
    Q_SIGNAL void storeChanged();

    QString filter();
    void setFilter(const QString& filter);
    Q_SIGNAL void filterChanged();

};
