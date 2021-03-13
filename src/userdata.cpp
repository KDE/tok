#include "userdata.h"

#include "client.h"

UserData::UserData(QObject* parent)
{

}

UserData::~UserData()
{

}

void UserData::componentComplete()
{
}

void UserData::doUpdate()
{
    if (!m_client) {
        return;
    }

    if (auto data = m_client->userData(m_userID)) {
        handleUpdate(m_userID, data);
        return;
    }

    m_client->call<TDApi::getUser>(
        [this](TDApi::getUser::ReturnType t) {
            handleUpdate(t->id_, t.get());
        },
        m_userID
    );
}

#define breakable for(int i = 0; i < 1; i++)

void UserData::handleUpdate(qint32 ID, TDApi::user* user)
{
    if (ID != m_userID) {
        return;
    }

    breakable {
        auto name = QString::fromStdString(user->first_name_ + " " + user->last_name_);
        if (name == m_name) {
            break;
        }

        m_name = name;
        Q_EMIT nameChanged();
    }
    breakable {
        if (user->profile_photo_ == nullptr) {
            if (m_smallAvatar == "") {
                break;
            }
            m_smallAvatar = "";
            Q_EMIT smallAvatarChanged();
            break;
        }

        auto url = QString("image://telegram/%1").arg(user->profile_photo_->small_->id_);
        if (m_smallAvatar == url) {
            break;
        }

        m_smallAvatar = url;
        Q_EMIT smallAvatarChanged();
    }

}

void UserData::setUserID(const QString& userID)
{
    m_userID = userID.toLong();
    Q_EMIT userIDChanged();
    doUpdate();
}

void UserData::setClient(Client* client)
{
    if (m_client == client) {
        return;
    }

    if (m_client != nullptr) {
        disconnect(m_client, &Client::userDataChanged, this, &UserData::handleUpdate);
    }

    m_client = client;
    Q_EMIT clientChanged();

    connect(m_client, &Client::userDataChanged, this, &UserData::handleUpdate);
    doUpdate();
}
