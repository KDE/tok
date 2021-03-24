#include <KLocalizedString>

#include "userdata.h"

#include "extractinator.h"

QString Extractinator::extractAuthor(Client* c, TDApi::message* msg)
{
    using namespace TDApi;

    match (msg->sender_)
        handleCase(messageSenderUser, user)
            getOrRet(data, c->userDataModel()->userData[user->user_id_], i18n("Unknown Sender"));

            return QStringList{QString::fromStdString(data->first_name_),QString::fromStdString(data->last_name_)}.join(" ").trimmed();
        endhandle
    endmatch

    return i18n("Unsupported");
}

QString Extractinator::extractBody(Client* c, TDApi::message* msg)
{
    using namespace TDApi;

    QString ret;

    match(msg->content_)
        handleCase(messageText, msg)
            ret = QString::fromStdString(msg->text_->text_);
            break;
        endhandle
        default: {
            ret = "Unsupported";
            break;
        }
    endmatch

    return ret;
}

QuickGlance Extractinator::extract(Client* c, TDApi::message* msg)
{
    return QuickGlance { extractAuthor(c, msg), extractBody(c, msg) };
}
