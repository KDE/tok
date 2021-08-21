#include "client_p.h"

SearchMessagesModel* Client::searchMessagesModel(QJsonObject params)
{
    Client* parent = this;
    TDApi::int53 chatID = params["chatID"].toString().toLongLong();
    std::string chatQuery = params["search"].toString().toStdString();
    std::function<TDApi::object_ptr<TDApi::SearchMessagesFilter>(void)> filter = [params]() -> TDApi::object_ptr<TDApi::SearchMessagesFilter> {
        if (params["kind"].toString() == "photos") {
            return TDApi::make_object<TDApi::searchMessagesFilterPhoto>();
        } else if (params["kind"].toString() == "videos") {
            return TDApi::make_object<TDApi::searchMessagesFilterVideo>();
        } else if (params["kind"].toString() == "audios") {
            return TDApi::make_object<TDApi::searchMessagesFilterAudio>();
        }
        return nullptr;
    };

    return new SearchMessagesModel(
        parent, chatID, chatQuery, filter
    );
    return nullptr;
}