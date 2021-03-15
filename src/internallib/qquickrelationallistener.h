#pragma once

#include <QObject>
#include <QQmlParserStatus>
#include <QQmlComponent>

#include "qabstractrelationalmodel.h"

class QQmlRelationalListenerPrivate;

class QQmlRelationalListener : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QAbstractRelationalModel* model READ model WRITE setModel RESET resetModel NOTIFY modelChanged)
    Q_PROPERTY(QVariant key READ key WRITE setKey RESET resetKey NOTIFY keyChanged)
    Q_PROPERTY(QQmlComponent* shape READ shape WRITE setShape NOTIFY shapeChanged)
    Q_PROPERTY(QObject* data READ data NOTIFY dataChanged)

public:
    explicit QQmlRelationalListener(QObject* parent = nullptr);
    ~QQmlRelationalListener();

    void classBegin() override {}
    void componentComplete() override;

    QAbstractRelationalModel* model() const;
    void setModel(QAbstractRelationalModel* setModel);
    void resetModel();
    Q_SIGNAL void modelChanged();

    QVariant key() const;
    void setKey(const QVariant& key);
    void resetKey();
    Q_SIGNAL void keyChanged();

    QQmlComponent* shape() const;
    void setShape(QQmlComponent* shape);
    Q_SIGNAL void shapeChanged();

    QObject* data() const;
    Q_SIGNAL void dataChanged();

private:
    Q_DECLARE_PRIVATE(QQmlRelationalListener)
    std::unique_ptr<QQmlRelationalListenerPrivate> d_ptr;

    void applyChanged(const QVector<int>& roles);
    void checkKey();

};