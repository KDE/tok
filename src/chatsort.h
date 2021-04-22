#pragma once

#include <QSortFilterProxyModel>

class ChatSortModel : public QSortFilterProxyModel
{

    Q_OBJECT

protected:
    bool lessThan(const QModelIndex& lhs, const QModelIndex& rhs) const override;

};
