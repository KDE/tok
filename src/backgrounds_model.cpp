#include "backgrounds_model_p.h"

BackgroundsModel::BackgroundsModel(Client* parent) : QAbstractListModel(parent), c(parent), d(new Private)
{
    c->call<TDApi::getBackgrounds>([=](auto r) {
        qWarning() << "bgs" << TDApi::to_string(r).c_str();
    });
}
BackgroundsModel::~BackgroundsModel()
{

}

QVariant BackgroundsModel::data(const QModelIndex& idx, int role) const
{

}
int BackgroundsModel::rowCount(const QModelIndex& parent) const
{

}
QHash<int,QByteArray> BackgroundsModel::roleNames() const
{

}
