// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "test_event_feeder.h"

#include "client.h"
#include "client_p.h"
#include <td/telegram/td_api.h>

TestEventFeeder::TestEventFeeder(Client* parent) : QObject(parent)
{
    c = parent;
    stages = {
        { "stageInitial", [this] { stageInitial(); } },

        { "stageEntryNumber",   [this] { stageEntryNumber(); } },
        { "stageEntryCode",     [this] { stageEntryCode(); } },
        { "stageEntryPassword", [this] { stageEntryPassword(); } },

        { "stageNoChats", [this] { stageNoChats(); } },
    };
}

void TestEventFeeder::triggerStage(const QString& s)
{
    stages[s]();
}

void TestEventFeeder::stageInitial()
{
}

void TestEventFeeder::stageEntryNumber()
{
    c->d->handleResponse(TD::ClientManager::Response {
        .object = TDApi::make_object<TDApi::updateAuthorizationState>(
            TDApi::make_object<TDApi::authorizationStateWaitPhoneNumber>()
        ),
    });
}

void TestEventFeeder::stageEntryCode()
{
    c->d->handleResponse(TD::ClientManager::Response {
        .object = TDApi::make_object<TDApi::updateAuthorizationState>(
            TDApi::make_object<TDApi::authorizationStateWaitCode>()
        ),
    });
}

void TestEventFeeder::stageEntryPassword()
{
    c->d->handleResponse(TD::ClientManager::Response {
        .object = TDApi::make_object<TDApi::updateAuthorizationState>(
            TDApi::make_object<TDApi::authorizationStateWaitPassword>()
        ),
    });
}

void TestEventFeeder::stageNoChats()
{
    c->d->handleResponse(TD::ClientManager::Response {
        .object = TDApi::make_object<TDApi::updateAuthorizationState>(
            TDApi::make_object<TDApi::authorizationStateReady>()
        ),
    });
}

