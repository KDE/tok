// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QQmlParserStatus>
#include <QQmlComponent>

#include "qabstractrelationalmodel.h"

class TokQmlRelationalListenerPrivate;

class TokQmlRelationalListener : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(TokAbstractRelationalModel* model READ model WRITE setModel RESET resetModel NOTIFY modelChanged)
    Q_PROPERTY(QVariant key READ key WRITE setKey RESET resetKey NOTIFY keyChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled RESET resetEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QQmlComponent* shape READ shape WRITE setShape NOTIFY shapeChanged)
    Q_PROPERTY(QObject* data READ data NOTIFY dataChanged)

public:
    explicit TokQmlRelationalListener(QObject* parent = nullptr);
    ~TokQmlRelationalListener();

    void classBegin() override {}
    void componentComplete() override;

    TokAbstractRelationalModel* model() const;
    void setModel(TokAbstractRelationalModel* setModel);
    void resetModel();
    Q_SIGNAL void modelChanged();

    QVariant key() const;
    void setKey(const QVariant& key);
    void resetKey();
    Q_SIGNAL void keyChanged();

    bool enabled() const;
    void setEnabled(bool enabled);
    void resetEnabled();
    Q_SIGNAL void enabledChanged();

    QQmlComponent* shape() const;
    void setShape(QQmlComponent* shape);
    Q_SIGNAL void shapeChanged();

    QObject* data() const;
    Q_SIGNAL void dataChanged();

private:
    Q_DECLARE_PRIVATE(TokQmlRelationalListener)
    std::unique_ptr<TokQmlRelationalListenerPrivate> d_ptr;

    void applyChanged(const QVector<int>& roles);
    void checkKey();
    void newRelationalModel(TokAbstractRelationalModel* model);

};
