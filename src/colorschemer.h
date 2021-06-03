#pragma once

#include <QObject>
#include <QIdentityProxyModel>

class QAbstractItemModel;
class KColorSchemeManager;

class RoleNames : public QIdentityProxyModel
{
    Q_OBJECT

public:
    QHash<int,QByteArray> roleNames() const override;
};

class ColorSchemer : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QAbstractItemModel* model READ model CONSTANT)

    KColorSchemeManager* c;

public:
    ColorSchemer(QObject* parent = nullptr);
    ~ColorSchemer();

    QAbstractItemModel* model() const;
    Q_INVOKABLE void apply(int idx);

};
