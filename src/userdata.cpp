// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "client.h"
#include "defs.h"
#include "overloader.h"
#include <KLocalizedString>
#include <td/telegram/td_api.h>

#include <unicode/reldatefmt.h>

#include "userdata.h"
#include "photoutils.h"

enum Roles {
    Name = Qt::UserRole,
    SmallAvatar,
    Bio,
    Username,
    Status,
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
        return imageToURL(userData[id]->profile_photo_->small_);
    case Roles::Username:
        return QString::fromStdString(userData[id]->username_);
    case Roles::Status:
        match(userData[id]->status_)
            handleCase(userStatusOnline, v)
                Q_UNUSED(v)
                return i18n("Online");
            endhandle
            handleCase(userStatusRecently, v)
                Q_UNUSED(v)
                return i18n("Online recently");
            endhandle
            handleCase(userStatusOffline, v)
                UErrorCode status = U_ZERO_ERROR;
                icu::UnicodeString appendTo;
                std::string converted;
                icu::RelativeDateTimeFormatter fmt(status);

                const auto time = QDateTime::fromTime_t(v->was_online_);
                const auto now = QDateTime::currentDateTime();
                const auto secsTo = time.secsTo(now);
                const auto minutesTo = secsTo / 60;
                const auto hoursTo = secsTo / 60 / 60;

                if (time.daysTo(now) > 0) {
                    fmt.format(time.daysTo(now), UDAT_DIRECTION_LAST, UDAT_RELATIVE_DAYS, appendTo, status);
                } else if (hoursTo > 0) {
                    fmt.format(hoursTo, UDAT_DIRECTION_LAST, UDAT_RELATIVE_HOURS, appendTo, status);
                } else if (minutesTo > 0) {
                    fmt.format(minutesTo, UDAT_DIRECTION_LAST, UDAT_RELATIVE_MINUTES, appendTo, status);
                } else {
                    fmt.format(secsTo, UDAT_DIRECTION_LAST, UDAT_RELATIVE_MINUTES, appendTo, status);
                }
                appendTo.toUTF8String(converted);

                return i18nc("%1 is a (translated via ICU) relative time, such as '3 seconds ago'", "Last online %1", QString::fromStdString(converted));
            endhandle
            default: {
                return i18n("Unknown Status");
            }
        endmatch
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
    ret[Roles::Status] = "status";

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
                qWarning() << QString::fromStdString(TDApi::to_string(update));
                qFatal("unhandled userdatamodel update");
            }));
}
