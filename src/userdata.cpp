#include "client.h"
#include "defs.h"
#include "overloader.h"

#include "userdata.h"

enum Roles {
    Name = Qt::UserRole,
    SmallAvatar,
};

UserDataModel::UserDataModel(Client* parent) : TokAbstractRelationalModel(parent), c(parent)
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

    switch (Roles(role)) {
    case Roles::Name:
        return QString::fromStdString(m_userData[id]->first_name_ + " " + m_userData[id]->last_name_);
    case Roles::SmallAvatar:
        if (m_userData[id]->profile_photo_ == nullptr) {
            return QVariant();
        }
        return QString("image://telegram/%1").arg(m_userData[id]->profile_photo_->small_->id_);
    }

    Q_UNREACHABLE();
}

bool UserDataModel::checkKey(const QVariant& key)
{
    return m_userData.contains(key.toString().toLong());
}

bool UserDataModel::canFetchKey(const QVariant& key)
{
    Q_UNUSED(key)

    return true;
}

void UserDataModel::fetchKey(const QVariant& key)
{
    c->call<TDApi::getUser>(
        [this, key](TDApi::getUser::ReturnType t) {
            m_userData[key.toString().toLong()] = std::move(t);
            keyAdded(key);
        },
        key.toString().toLong()
    );
}

QHash<int, QByteArray> UserDataModel::roleNames()
{
    QHash<int,QByteArray> ret;

    ret[Roles::Name] = "name";
    ret[Roles::SmallAvatar] = "smallAvatar";

    return ret;
}

void UserDataModel::handleUpdate(TDApi::object_ptr<TDApi::Update> u)
{
    TDApi::downcast_call(*u,
        overloaded(
            [this](TDApi::updateUser &update_user) {
                auto id = update_user.user_->id_;
                auto contained = m_userData.contains(id);
                m_userData[id] = std::move(update_user.user_);
                if (contained) {
                    Q_EMIT keyDataChanged(QString::number(id), {});
                } else {
                    Q_EMIT keyAdded(QString::number(id));
                }
            },
            [](auto& update) { qWarning() << "unhandled userdatamodel update" << QString::fromStdString(TDApi::to_string(update)); }));
}
