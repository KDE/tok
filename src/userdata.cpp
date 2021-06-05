// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "client.h"
#include "defs.h"
#include "overloader.h"
#include <KLocalizedString>
#include <td/telegram/td_api.h>

#include "userdata.h"

enum Roles {
    Name = Qt::UserRole,
    SmallAvatar,
    Bio,
    Username,
};

UserDataModel::UserDataModel(Client* parent)
    : TokAbstractRelationalModel(parent)
    , c(parent)
{
}

UserDataModel::~UserDataModel()
{
}

QVariant UserDataModel::data(const QVariant& key, int role)
{
    if (!checkKey(key)) {
        return QVariant();
    }

    auto id = key.toString().toLong();

    using namespace TDApi;

    switch (Roles(role)) {
    case Roles::Bio:
        if (!fullUserData.contains(id)) {
            c->call<getUserFullInfo>(
                [this, id = id](getUserFullInfo::ReturnType ret) {
                    fullUserData[id] = std::move(ret);
                    Q_EMIT keyAdded(QString::number(id));
                },
                id);
            return i18n("Loading…");
        }
        return QString::fromStdString(fullUserData[id]->bio_);
    case Roles::Name:
        return QString::fromStdString(userData[id]->first_name_ + " " + userData[id]->last_name_).trimmed();
    case Roles::SmallAvatar:
        if (userData[id]->profile_photo_ == nullptr) {
            return QVariant();
        }
        return QString("image://telegram/%1").arg(userData[id]->profile_photo_->small_->id_);
    case Roles::Username:
        return QString::fromStdString(userData[id]->username_);
    }

    Q_UNREACHABLE();
}

bool UserDataModel::checkKey(const QVariant& key)
{
    return userData.contains(key.toString().toLong());
}

bool UserDataModel::canFetchKey(const QVariant& key)
{
    Q_UNUSED(key)

    return true;
}

void UserDataModel::fetchKey(const QVariant& key)
{
    if (key.toString().toLong() == 0) {
        return;
    }
    c->call<TDApi::getUser>(
        [this, key](TDApi::getUser::ReturnType t) {
            userData[key.toString().toLong()] = std::move(t);
            keyAdded(key);
        },
        key.toString().toLong());
}

QHash<int, QByteArray> UserDataModel::roleNames()
{
    QHash<int, QByteArray> ret;

    ret[Roles::Name] = "name";
    ret[Roles::SmallAvatar] = "smallAvatar";
    ret[Roles::Bio] = "bio";
    ret[Roles::Username] = "username";

    return ret;
}

void UserDataModel::handleUpdate(TDApi::object_ptr<TDApi::Update> u)
{
    TDApi::downcast_call(*u,
        overloaded(
            [this](TDApi::updateUser& update_user) {
                auto id = update_user.user_->id_;
                auto contained = userData.contains(id);
                userData[id] = std::move(update_user.user_);
                if (contained) {
                    Q_EMIT keyDataChanged(QString::number(id), {});
                } else {
                    Q_EMIT keyAdded(QString::number(id));
                }
            },
            [this](TDApi::updateUserFullInfo& update_user_full_info) {
                auto id = update_user_full_info.user_id_;
                auto contained = userData.contains(id);
                fullUserData[id] = std::move(update_user_full_info.user_full_info_);
                if (contained) {
                    Q_EMIT keyDataChanged(QString::number(id), {});
                } else {
                    Q_EMIT keyAdded(QString::number(id));
                }
            },
            [](auto& update) {
                qWarning() << "unhandled userdatamodel update" << QString::fromStdString(TDApi::to_string(update));
            }));
}
