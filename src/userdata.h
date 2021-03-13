#pragma once

#include <QObject>
#include <QQmlParserStatus>

#include "defs.h"
#include "client.h"

class Client;

class UserData : public QObject, public QQmlParserStatus
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString userID READ userID WRITE setUserID NOTIFY userIDChanged)
    Q_PROPERTY(Client* client READ client WRITE setClient NOTIFY clientChanged)

public:
    UserData(QObject* parent = nullptr);
    ~UserData();

    void classBegin() override {};
    void componentComplete() override;

    QString name() const { return m_name; }
    QString userID() const { return QString::number(m_userID); }
    Client* client() const { return m_client; }

    void setUserID(const QString& userID);
    void setClient(Client* client);

    Q_SIGNAL void nameChanged();
    Q_SIGNAL void userIDChanged();
    Q_SIGNAL void clientChanged();

private:
    Client* m_client = nullptr;
    QString m_name = "Loading...";
    qint32 m_userID = 0;

    void doUpdate();
    void handleUpdate(qint32 userID, TDApi::user* user);

};
