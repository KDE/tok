// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QHash>
#include <QObject>

class TokAbstractRelationalModel : public QObject
{

    Q_OBJECT

public:
    explicit TokAbstractRelationalModel(QObject* parent = nullptr)
        : QObject(parent)
    {
        connect(this, &TokAbstractRelationalModel::keyAdded, this, [this](const QVariant& key) {
            Q_EMIT(keyDataChanged(key, {}));
        });
        connect(this, &TokAbstractRelationalModel::keyRemoved, this, [this](const QVariant& key) {
            Q_EMIT(keyDataChanged(key, {}));
        });
    }
    virtual ~TokAbstractRelationalModel() {};

    virtual QVariant data(const QVariant& key, int role = Qt::DisplayRole) = 0;

    // check whether or not 'key' is a valid key
    virtual bool checkKey(const QVariant& key) = 0;

    virtual bool canFetchKey(const QVariant& key)
    {
        return false;
    }
    virtual void fetchKey(const QVariant& key)
    {
        Q_UNUSED(key)
    }

    virtual QHash<int, QByteArray> roleNames()
    {
        return {
            { Qt::DisplayRole, "display" },
            { Qt::DecorationRole, "decoration" },
            { Qt::EditRole, "edit" },
            { Qt::ToolTipRole, "toolTip" },
            { Qt::StatusTipRole, "statusTip" },
            { Qt::WhatsThisRole, "whatsThis" },
        };
    }

Q_SIGNALS:
    // automatically emit keyDataChanged
    void keyAdded(const QVariant& key);
    void keyRemoved(const QVariant& key);

    void keyDataChanged(const QVariant& key, const QVector<int>& roles);
};
