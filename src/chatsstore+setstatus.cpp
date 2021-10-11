// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "chatsstore_p.h"

void ChatsStore::setStatus(const QString& chatID, const QString& userID, QJsonObject params)
{
    using namespace TDApi;

    const auto chatID_ = chatID.toLongLong();
    const auto userID_ = userID.toLongLong();

    object_ptr<ChatMemberStatus> status;

    if (params["status"] == "banned") {
        status = make_object<chatMemberStatusBanned>(0);
    } else if (params["status"] == "kicked") {
        status = make_object<chatMemberStatusLeft>();
    }

    c->call<setChatMemberStatus>([](auto r) {}, chatID_, make_object<messageSenderUser>(userID_), std::move(status));
}