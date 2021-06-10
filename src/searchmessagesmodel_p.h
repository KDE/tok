#pragma once

#include "searchmessagesmodel.h"

struct SearchMessagesModel::Private
{
    TDApi::int53 chatID;
    std::string chatQuery;
    std::function<TDApi::object_ptr<TDApi::MessageSender>(void)> sender;
    std::function<TDApi::object_ptr<TDApi::SearchMessagesFilter>(void)> filter;

    TDApi::array<TDApi::object_ptr<TDApi::message>> messages;

    bool atEnd;
};
