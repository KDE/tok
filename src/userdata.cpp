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
        m_name = QString::fromStdString(data->username_);
        Q_EMIT nameChanged();

        return;
    }

    m_client->call<TDApi::getUser>(
        [this](TDApi::getUser::ReturnType t) {
            handleUpdate(t->id_, t.get());
        },
        m_userID
    );
}

void UserData::handleUpdate(qint32 ID, TDApi::user* user)
{
    if (ID != m_userID) {
        return;
    }

    auto name = QString::fromStdString(user->username_);
    if (name == m_name) {
        return;
    }

    m_name = name;
    Q_EMIT nameChanged();
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
