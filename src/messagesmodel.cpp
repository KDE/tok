#include "messagesmodel_p.h"

enum Roles {
    Content = Qt::UserRole,
    AuthorID,
    PreviousAuthorID,
    NextAuthorID,
};

MessagesModel::MessagesModel(Client* parent, TDApi::int53 id) : QAbstractListModel(parent), c(parent), d(new Private)
{
    d->id = id;

    fetch();
}

MessagesModel::~MessagesModel()
{
}

bool MessagesModel::canFetchMore(const QModelIndex& parent) const
{
    return true;
}

void MessagesModel::fetchMore(const QModelIndex& parent)
{
    fetch();
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
            dataChanged(index(0), index(0), {Roles::PreviousAuthorID, Roles::NextAuthorID});
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

    auto idFrom = [](TDApi::MessageSender* s) {
        switch (s->get_id()) {
        case TDApi::messageSenderUser::ID: {
            auto moved = static_cast<TDApi::messageSenderUser*>(s);
            return QString::number(moved->user_id_);
        }
        case TDApi::messageSenderChat::ID: {
            return QString();
        }
        }

        return QString();
    };

    switch (r) {
    case Roles::Content: {
        switch (d->messageData[mID]->content_->get_id()) {
        case TDApi::messageText::ID: {
            auto moved = static_cast<TDApi::messageText*>(d->messageData[mID]->content_.get());
            return QString::fromStdString(moved->text_->text_);
        }
        }
    }

    case Roles::AuthorID: {
        return idFrom(d->messageData[mID]->sender_.get());
    }

    case Roles::PreviousAuthorID: {
        if (!(int(d->messages.size()) > idx.row()+1)) {
            return QString();
        }

        auto mPrevID = d->messages[idx.row()+1];
        if (!d->messageData.contains(mPrevID)) {
            return QString();
        }

        return idFrom(d->messageData[mPrevID]->sender_.get());
    }

    case Roles::NextAuthorID: {
        if (idx.row() - 1 < 0) {
            return QString();
        }

        auto mNextID = d->messages[idx.row()-1];
        if (!d->messageData.contains(mNextID)) {
            return QString();
        }

        return idFrom(d->messageData[mNextID]->sender_.get());
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
    roles[Roles::AuthorID] = "mAuthorID";
    roles[Roles::PreviousAuthorID] = "mPreviousAuthorID";
    roles[Roles::NextAuthorID] = "mNextAuthorID";

    return roles;
}

void MessagesModel::newMessage(TDApi::object_ptr<TDApi::message> msg)
{
    beginInsertRows(QModelIndex(), 0, 0);
    d->messages.push_front(msg->id_);
    d->messageData[msg->id_] = std::move(msg);
    endInsertRows();

    dataChanged(index(1), index(1), {Roles::PreviousAuthorID, Roles::NextAuthorID});
}

