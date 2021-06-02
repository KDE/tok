#include "membersmodel_p.h"

enum Roles {
    UserID,
};

MembersModel::MembersModel(Client* parent, TDApi::int53 chatID, bool isSupergroup) : QAbstractListModel(parent), c(parent), d(new Private)
{
    d->chatID = chatID;

    if (isSupergroup) {
        c->call<TDApi::getSupergroupMembers>(
            [this](TDApi::getSupergroupMembers::ReturnType r) {
                beginResetModel();
                d->members = std::move(r->members_);
                endResetModel();
            },
            chatID, TDApi::make_object<TDApi::supergroupMembersFilterRecent>(), 0, 200
        );
    } else {
        c->call<TDApi::getBasicGroupFullInfo>(
            [this](TDApi::getBasicGroupFullInfo::ReturnType r) {
                beginResetModel();
                d->members = std::move(r->members_);
                endResetModel();
            },
            chatID
        );
    }
}

MembersModel::~MembersModel()
{

}

bool MembersModel::canFetchMore(const QModelIndex& parent) const
{
    return d->isSupergroup && !d->atEnd;
}

void MembersModel::fetchMore(const QModelIndex& parent)
{

}


QVariant MembersModel::data(const QModelIndex& idx, int role) const
{
    auto r = idx.row();
    if (r >= d->members.size()) {
        return QVariant();
    }

    switch (Roles(role)) {
    case Roles::UserID:
        return QString::number(static_cast<TDApi::messageSenderUser*>(d->members[r]->member_id_.get())->user_id_);
    }

    return QVariant();
}

int MembersModel::rowCount(const QModelIndex& parent) const
{
    return d->members.size();
}

QHash<int,QByteArray> MembersModel::roleNames() const
{
    return {
        { Roles::UserID, "userID" },
    };
}

