#include "messagesmodel_p.h"

enum Roles {
    Content = Qt::UserRole,
};

MessagesModel::MessagesModel(Client* parent, TDApi::int53 id) : QAbstractListModel(parent), c(parent), d(new Private)
{
    d->id = id;
}

MessagesModel::~MessagesModel()
{
}

void MessagesModel::fetch()
{
    c->call<TDApi::getChatHistory>(
        [=, this](TDApi::getChatHistory::ReturnType resp) {
            beginInsertRows(QModelIndex(), d->messages.size(), d->messages.size()+resp->messages_.size()-1);
            for (auto& msg : resp->messages_) {
                d->messages.push_back(msg->id_);
                d->messageData[msg->id_] = std::move(msg);
            }
            endInsertRows();
        },
        d->id, d->messages.empty() ? 0 : d->messageData[d->messages[d->messages.size()-1]]->id_, 0, 50, false
    );
}

QVariant MessagesModel::data(const QModelIndex& idx, int role) const
{
    if (!checkIndex(idx, CheckIndexOption::IndexIsValid)) {
        return QVariant();
    }

    auto mID = d->messages[idx.row()];
    if (!d->messageData.contains(mID)) {
        return QVariant();
    }

    Roles r = Roles(role);

    switch (r) {
    case Roles::Content: {
        switch (d->messageData[mID]->content_->get_id()) {
        case TDApi::messageText::ID: {
            auto moved = static_cast<TDApi::messageText*>(d->messageData[mID]->content_.get());
            return QString::fromStdString(moved->text_->text_);
        }
        }
    }
    }

    return QVariant();
}

int MessagesModel::rowCount(const QModelIndex& parent) const
{
    return d->messages.size();
}

QHash<int,QByteArray> MessagesModel::roleNames() const
{
    QHash<int,QByteArray> roles;

    roles[Roles::Content] = "mContent";

    return roles;
}

