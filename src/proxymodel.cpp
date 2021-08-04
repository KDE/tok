// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "proxymodel_p.h"

enum Roles {
    Server,
    Port,
    Enabled,
    Kind,
    MTProto_Secret,
    HTTP_Username,
    HTTP_Password,
    HTTP_HTTPOnly,
    SOCKS5_Username,
    SOCKS5_Password,

    Deleted,
};

ProxyModel::ProxyModel(Client* parent) : QAbstractListModel(parent), c(parent), d(new Private)
{
    c->call<TDApi::getProxies>([=](TDApi::getProxies::ReturnType r) {
        beginResetModel();
        d->proxies = std::move(r->proxies_);
        endResetModel();
    });
}
ProxyModel::~ProxyModel()
{

}
inline QString operator*(const TDApi::string& op)
{
    return QString::fromStdString(op);
}
inline TDApi::string operator*(const QString& op)
{
    return op.toStdString();
}
template<typename T, typename E>
const TDApi::object_ptr<T>& as(const E& foo)
{
    return reinterpret_cast<const TDApi::object_ptr<T>&>(foo->type_);
}
QVariant ProxyModel::data(const QModelIndex& idx, int role) const
{
    auto r = idx.row();
    if (r >= d->proxies.size()) {
        return QVariant();
    }
    const auto& data = d->proxies[r];

    switch (Roles(role)) {
    case Server:
        return *data->server_;
    case Port:
        return data->port_;
    case Enabled:
        return data->is_enabled_;
    case Kind:
        switch (data->type_->get_id()) {
        case TDApi::proxyTypeHttp::ID: return "http";
        case TDApi::proxyTypeMtproto::ID: return "mtproto";
        case TDApi::proxyTypeSocks5::ID: return "socks5";
        }
    case MTProto_Secret:
        return *as<TDApi::proxyTypeMtproto>(data)->secret_;
    case HTTP_Username:
        return *as<TDApi::proxyTypeHttp>(data)->username_;
    case HTTP_Password:
        return *as<TDApi::proxyTypeHttp>(data)->password_;
    case HTTP_HTTPOnly:
        return as<TDApi::proxyTypeHttp>(data)->http_only_;
    case SOCKS5_Username:
        return *as<TDApi::proxyTypeSocks5>(data)->username_;
    case SOCKS5_Password:
        return *as<TDApi::proxyTypeSocks5>(data)->password_;
    case Deleted:
        return false;
    }

    return QVariant();
}
bool ProxyModel::setData(const QModelIndex& idx, const QVariant& dat, int role)
{
    auto r = idx.row();
    if (r >= d->proxies.size()) {
        return false;
    }
    auto& data = d->proxies[r];

    auto broadcast = [this, id = data->id_]() {
        for (auto i = 0UL; i < d->proxies.size(); i++) {
            if (d->proxies[i]->id_ == id) {
                Q_EMIT dataChanged(index(i), index(i));
                return;
            }
        }
    };

    switch (Roles(role)) {
    case Deleted:
        c->call<TDApi::removeProxy>(
            [this, id = data->id_](TDApi::removeProxy::ReturnType) {
                for (auto i = 0UL; i < d->proxies.size(); i++) {
                    if (d->proxies[i]->id_ == id) {
                        beginRemoveRows(QModelIndex(), i, i);
                        d->proxies.erase(d->proxies.begin() + i);
                        endRemoveRows();
                        break;
                    }
                }
            },
            data->id_
        );
        return true;
    case Enabled:
        c->call<TDApi::editProxy>(
            [broadcast, &data](TDApi::editProxy::ReturnType r) mutable {
                data.reset(r.release());
                broadcast();
            },
            data->id_, data->server_, data->port_, dat.toBool(), std::move(data->type_)
        );
        return true;
    default:
        return false;
    }
}
int ProxyModel::rowCount(const QModelIndex& parent) const
{
    return d->proxies.size();
}
void ProxyModel::insert(
    const QString& server,
    int port,
    bool enabled,
    const QJsonObject& otherData
) {
    using namespace TDApi;

    object_ptr<ProxyType> kind;

    if (otherData["kind"] == "http") {
        kind.reset(new proxyTypeHttp(
            *otherData["http_Username"].toString(),
            *otherData["http_Password"].toString(),
            otherData["http_HTTPOnly"].toBool()
        ));
    } else if (otherData["kind"] == "mtproto") {
        kind.reset(new proxyTypeMtproto(
            *otherData["mtproto_Secret"].toString()
        ));
    } else if (otherData["kind"] == "socks5") {
        kind.reset(new proxyTypeSocks5(
            *otherData["socks55_Username"].toString(),
            *otherData["socks55_Password"].toString()
        ));
    }

    c->call<addProxy>(
        [this](addProxy::ReturnType r) {
            beginInsertRows(QModelIndex(), d->proxies.size(), d->proxies.size());
            d->proxies.push_back(std::move(r));
            endInsertRows();
        },
        *server, port, enabled, std::move(kind)
    );
}
QHash<int,QByteArray> ProxyModel::roleNames() const
{
    return {
        { Server, "server" },
        { Port, "port" },
        { Enabled, "enabled" },
        { Kind, "kind" },
        { MTProto_Secret, "mtproto_Secret" },
        { HTTP_Username, "http_Username" },
        { HTTP_Password, "http_Password" },
        { HTTP_HTTPOnly, "http_HTTPOnly" },
        { SOCKS5_Username, "socks55_Username" },
        { SOCKS5_Password, "socks55_Password" },
        { Deleted, "deleted" },
    };
}
