#include "searchmessagesmodel_p.h"

SearchMessagesModel::SearchMessagesModel(Client* parent, TDApi::int53 chatID, std::string chatQuery, std::function<TDApi::object_ptr<TDApi::SearchMessagesFilter>(void)> filter, std::function<TDApi::object_ptr<TDApi::MessageSender>(void)> sender) : QAbstractListModel(parent), c(parent), d(new Private)
{
    d->chatID = chatID;
    d->chatQuery = chatQuery;
    d->sender = std::move(sender);
    d->filter = std::move(filter);
}

SearchMessagesModel::~SearchMessagesModel()
{
}


bool SearchMessagesModel::canFetchMore(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return !d->atEnd;
}

void SearchMessagesModel::fetchMore(const QModelIndex& parent)
{
    Q_UNUSED(parent)

    auto from = d->messages.empty() ? 0 : d->messages[d->messages.size()]->id_;

    c->call<TDApi::searchChatMessages>([this](TDApi::searchChatMessages::ReturnType r) {
        if (r->messages_.empty()) {
            d->atEnd = true;
            return;
        }

        beginInsertRows(QModelIndex(), d->messages.size(), d->messages.size() + r->messages_.size());
        for (auto& it : r->messages_) {
            d->messages.push_back(std::move(it));
        }
        endInsertRows();
    }, d->chatID, d->chatQuery, d->sender(), from, 0, 50, d->filter(), 0);
}

QVariant SearchMessagesModel::data(const QModelIndex& idx, int role) const
{
    auto r = idx.row();

    if (role != 0 || r >= d->messages.size()) {
        return QVariant();
    }

    return QString::number(d->messages[r]->id_);
}

int SearchMessagesModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return d->messages.size();
}

QHash<int,QByteArray> SearchMessagesModel::roleNames() const
{
    return {{0, "messageID"}};
}