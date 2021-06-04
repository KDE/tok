#pragma once

#include "internallib/qabstractrelationalmodel.h"
#include "yoinked from qt automotive/qivipendingreply.h"

#include "client.h"

class FileMangler : public TokAbstractRelationalModel
{

    Q_OBJECT

    Client* c;

    struct Private;
    std::unique_ptr<Private> d;

public:

    FileMangler(Client* parent);
    ~FileMangler();

    QVariant data(const QVariant& key, int role = Qt::DisplayRole) override;
    bool checkKey(const QVariant& key) override;

    QHash<int,QByteArray> roleNames() override;

    Q_INVOKABLE QIviPendingReplyBase downloadFile(const QString& id);

};
