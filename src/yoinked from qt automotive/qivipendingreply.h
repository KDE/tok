/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
** SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QIVIPENDINGREPLY_H
#define QIVIPENDINGREPLY_H

#include <QJSValue>
#include <QObject>
#include <QSharedPointer>
#include <QVariant>
#include <QDebug>
#include <QMetaEnum>

#define Q_QTIVICORE_EXPORT

QT_BEGIN_NAMESPACE

class QIviPendingReplyWatcherPrivate;

Q_QTIVICORE_EXPORT void qiviRegisterPendingReplyBasicTypes();

class Q_QTIVICORE_EXPORT QIviPendingReplyWatcher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant value READ value NOTIFY valueChanged)
    Q_PROPERTY(bool valid READ isValid CONSTANT)
    Q_PROPERTY(bool resultAvailable READ isResultAvailable NOTIFY valueChanged)
    Q_PROPERTY(bool success READ isSuccessful NOTIFY valueChanged)

public:
    QVariant value() const;
    bool isValid() const;
    bool isResultAvailable() const;
    bool isSuccessful() const;

    Q_INVOKABLE void setSuccess(const QVariant &value);
    Q_INVOKABLE void setFailed();
    Q_INVOKABLE void then(const QJSValue &success, const QJSValue &failed = QJSValue());

Q_SIGNALS:
    void replyFailed();
    void replySuccess();
    void valueChanged(const QVariant &value);

private:
    explicit QIviPendingReplyWatcher(int userType);
    Q_DECLARE_PRIVATE(QIviPendingReplyWatcher)
    friend class QIviPendingReplyBase;
};

class Q_QTIVICORE_EXPORT QIviPendingReplyBase
{
    Q_GADGET
    Q_PROPERTY(QIviPendingReplyWatcher* watcher READ watcher)
    Q_PROPERTY(QVariant value READ value)
    Q_PROPERTY(bool valid READ isValid)
    Q_PROPERTY(bool resultAvailable READ isResultAvailable)
    Q_PROPERTY(bool success READ isSuccessful)

public:
    explicit QIviPendingReplyBase(int userType);
    QIviPendingReplyBase() = default;
    QIviPendingReplyBase(const QIviPendingReplyBase & other);
    ~QIviPendingReplyBase() = default;
    QIviPendingReplyBase& operator=(const QIviPendingReplyBase&) = default;
    QIviPendingReplyBase& operator=(QIviPendingReplyBase&&) = default;

    QIviPendingReplyWatcher* watcher() const;
    QVariant value() const;
    bool isValid() const;
    bool isResultAvailable() const;
    bool isSuccessful() const;

    Q_INVOKABLE void then(const QJSValue &success, const QJSValue &failed = QJSValue());
    Q_INVOKABLE void setSuccess(const QVariant & value);
    Q_INVOKABLE void setFailed();

protected:
    void setSuccessNoCheck(const QVariant & value);

    QSharedPointer<QIviPendingReplyWatcher> m_watcher;
};

template <typename T> class QIviPendingReply : public QIviPendingReplyBase
{
public:
    QIviPendingReply(const T &successValue)
        : QIviPendingReply()
    {
        setSuccess(successValue);
    }

    QIviPendingReply()
        : QIviPendingReplyBase(qMetaTypeId<T>())
    {}

    using QIviPendingReplyBase::setSuccess;

    void setSuccess(const T &val)
    {
        setSuccessNoCheck(QVariant::fromValue(val));
    }

    T reply() const { return m_watcher->value().template value<T>(); }

    using QIviPendingReplyBase::then;

    void then(const std::function<void(const T &)> &success, const std::function<void()> &failed = std::function<void()>()) {
        if (isResultAvailable()) {
            if (isSuccessful() && success)
                success(reply());
            else if (failed)
                failed();
        } else {
            QSharedPointer<QIviPendingReplyWatcher> w = m_watcher;
            if (success) {
                QObject::connect(watcher(), &QIviPendingReplyWatcher::replySuccess, watcher(), [success, w]() {
                    success(w->value().value<T>());
                });
            }
            if (failed) {
                QObject::connect(watcher(), &QIviPendingReplyWatcher::replyFailed, watcher(), [failed]() {
                    failed();
                });
            }
        }
    }

    static QIviPendingReply createFailedReply()
    {
        QIviPendingReply<T> reply;
        reply.setFailed();
        return reply;
    }
};

template <> class QIviPendingReply <QVariant> : public QIviPendingReplyBase
{
public:
    QIviPendingReply(const QVariant &successValue)
        : QIviPendingReply()
    {
        setSuccess(successValue);
    }

    QIviPendingReply()
        : QIviPendingReplyBase(qMetaTypeId<QVariant>())
    {}

    void setSuccess(const QVariant &val)
    {
        setSuccessNoCheck(val);
    }

    QVariant reply() const { return m_watcher->value(); }

    void then(const std::function<void(const QVariant &)> &success, const std::function<void()> &failed = std::function<void()>()) {
        if (isResultAvailable()) {
            if (isSuccessful() && success)
                success(reply());
            else if (failed)
                failed();
        } else {
            QSharedPointer<QIviPendingReplyWatcher> w = m_watcher;
            if (success) {
                QObject::connect(watcher(), &QIviPendingReplyWatcher::replySuccess, watcher(), [success, w]() {
                    success(w->value());
                });
            }
            if (failed) {
                QObject::connect(watcher(), &QIviPendingReplyWatcher::replyFailed, watcher(), [failed]() {
                    failed();
                });
            }
        }
    }

    static QIviPendingReply createFailedReply()
    {
        QIviPendingReply<QVariant> reply;
        reply.setFailed();
        return reply;
    }
};

template <> class QIviPendingReply <void> : public QIviPendingReplyBase
{
public:
    QIviPendingReply()
        : QIviPendingReplyBase(qMetaTypeId<void>())
    {}

    using QIviPendingReplyBase::setSuccess;

    void setSuccess()
    {
        setSuccessNoCheck(QVariant());
    }

    void reply() const { return; }

    void then(const std::function<void()> &success, const std::function<void()> &failed = std::function<void()>()) {
        if (isResultAvailable()) {
            if (isSuccessful() && success)
                success();
            else if (failed)
                failed();
        } else {
            QSharedPointer<QIviPendingReplyWatcher> w = m_watcher;
            if (success) {
                QObject::connect(watcher(), &QIviPendingReplyWatcher::replySuccess, watcher(), [success, w]() {
                    success();
                });
            }
            if (failed) {
                QObject::connect(watcher(), &QIviPendingReplyWatcher::replyFailed, watcher(), [failed]() {
                    failed();
                });
            }
        }
    }

    static QIviPendingReply createFailedReply()
    {
        QIviPendingReply<void> reply;
        reply.setFailed();
        return reply;
    }
};

//Workaround for QTBUG-83664
//If T is a enum
template <typename T> Q_INLINE_TEMPLATE typename std::enable_if<QtPrivate::IsQEnumHelper<T>::Value, void>::type qIviRegisterPendingReplyType(const char *name = nullptr)
{
    qRegisterMetaType<T>();
    QString n;
    if (name) {
        n = QLatin1String(name);
    } else {
        QMetaEnum me = QMetaEnum::fromType<T>();
        if (me.isValid() && me.isFlag())
            n = QLatin1String(me.scope()) + QStringLiteral("::") + QLatin1String(me.name());
        else
            n = QLatin1String(QMetaType(qMetaTypeId<T>()).name());
    }

    const QString t_name = QStringLiteral("QIviPendingReply<") + n + QStringLiteral(">");
    qRegisterMetaType<QIviPendingReplyBase>(qPrintable(t_name));
}

//If T is NOT a enum
template <typename T> Q_INLINE_TEMPLATE typename std::enable_if<!QtPrivate::IsQEnumHelper<T>::Value, void>::type qIviRegisterPendingReplyType(const char *name = nullptr)
{
    qRegisterMetaType<T>();
    const char* n = name ? name : QMetaType(qMetaTypeId<T>()).name().constData();
    const QString t_name = QStringLiteral("QIviPendingReply<") + QLatin1String(n) + QStringLiteral(">");
    qRegisterMetaType<QIviPendingReplyBase>(qPrintable(t_name));
}

QT_END_NAMESPACE

#endif // QIVIPENDINGREPLY_H
