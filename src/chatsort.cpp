#include <QDebug>

#include "chatsort.h"

bool ChatSortModel::lessThan(const QModelIndex& lhs, const QModelIndex& rhs) const
{
    qWarning() << "sortinate...";

    auto lhID = sourceModel()->data(lhs, Qt::UserRole);
    auto lhOrd = sourceModel()->data(lhs, Qt::UserRole+1);

    auto rhID = sourceModel()->data(rhs, Qt::UserRole);
    auto rhOrd = sourceModel()->data(rhs, Qt::UserRole+1);

    return qMakePair(lhOrd, lhID) < qMakePair(rhOrd, rhID);
};
