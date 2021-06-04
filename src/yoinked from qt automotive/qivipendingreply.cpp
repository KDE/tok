/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
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
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#include "qivipendingreply.h"
#include "qivipendingreply_p.h"
#include "qiviqmlconversion_helper.h"

#include "private/qjsengine_p.h"
#include "private/qjsvalue_p.h"

#include <QDebug>
#include <QJSEngine>
#include <QtQml>

QT_BEGIN_NAMESPACE

#define QTIVI_ADD_STATIC_METATYPE2(MetaTypeName, MetaTypeId, AliasingType, RealName) \
    qIviRegisterPendingReplyType<AliasingType>(RealName);

#define QTIVI_ADD_STATIC_METATYPE(MetaTypeName, MetaTypeId, AliasingType) \
    QTIVI_ADD_STATIC_METATYPE2(MetaTypeName, MetaTypeId, AliasingType, nullptr)


/*!
    \relates QIviPendingReply

    Registers QIviPendingReplys of all Qt basic types to the meta type system.

    Usually this function called automatically when creating a QCoreApplication or a QIviPendingReply
    and doesn't need to be called manually.
*/
void qiviRegisterPendingReplyBasicTypes() {
    static bool once = false;
    if (once)
        return;

    // This function is registered as Q_COREAPP_STARTUP_FUNCTION, which makes sure
    // it is run after the QCoreApplication constructor to ensure we can register
    // types.
    // In case the library is loaded at runtime (because of a qml plugin dependency),
    // the init function would be registered and executed right away before the
    // rest of the library is initialized (e.g. the QMetaObject of QIviPendingReplyBase).
    // The singleshot timer makes sure the registration is done in the next event
    // loop run, when everything is ready.
    QMetaObject::invokeMethod(qApp, []() {
        qRegisterMetaType<QIviPendingReplyBase>("QIviPendingReplyBase");
        qRegisterMetaType<QIviPendingReply<bool>>("QIviPendingReply<bool>");
        QT_FOR_EACH_STATIC_PRIMITIVE_TYPE(QTIVI_ADD_STATIC_METATYPE)
        QT_FOR_EACH_STATIC_PRIMITIVE_POINTER(QTIVI_ADD_STATIC_METATYPE)
        QT_FOR_EACH_STATIC_CORE_POINTER(QTIVI_ADD_STATIC_METATYPE)
        QT_FOR_EACH_STATIC_CORE_TEMPLATE(QTIVI_ADD_STATIC_METATYPE)
        QT_FOR_EACH_STATIC_CORE_CLASS(QTIVI_ADD_STATIC_METATYPE)
        QT_FOR_EACH_STATIC_ALIAS_TYPE(QTIVI_ADD_STATIC_METATYPE2)
    });
    once = true;
}

Q_COREAPP_STARTUP_FUNCTION(qiviRegisterPendingReplyBasicTypes)

// TODO make it reentrant

QIviPendingReplyWatcherPrivate::QIviPendingReplyWatcherPrivate(int userType, QIviPendingReplyWatcher *parent)
    : QObjectPrivate()
    , q_ptr(parent)
    , m_type(userType)
    , m_resultAvailable(false)
    , m_success(false)
    , m_callbackEngine(nullptr)
{

}

void QIviPendingReplyWatcherPrivate::setSuccess(const QVariant &value)
{
    Q_Q(QIviPendingReplyWatcher);

    if (m_resultAvailable) {
        qWarning("Result is already set. Ignoring request");
        return;
    }

    m_resultAvailable = true;
    m_data = value;
    m_success = true;
    Q_EMIT q->valueChanged(value);
    Q_EMIT q->replySuccess();

    callSuccessCallback();
}

void QIviPendingReplyWatcherPrivate::callSuccessCallback()
{
    if (!m_successFunctor.isUndefined() && m_callbackEngine) {
        QJSValueList list = { m_callbackEngine->toScriptValue(m_data) };
        m_successFunctor.call(list);
    }
}

void QIviPendingReplyWatcherPrivate::callFailedCallback()
{
    if (!m_failedFunctor.isUndefined() && m_callbackEngine)
        m_failedFunctor.call();

    if (m_failedFunctor.isUndefined() && !m_successFunctor.isUndefined()) {
        //Logging category for logging unhandled failed functor with location.
    }
}

/*!
    \class QIviPendingReplyWatcher
    \inmodule QtIviCore
    \brief The QIviPendingReplyWatcher provides signals for QIviPendingReply.

    The QIviPendingReplyWatcher holds all data of a QIviPendingReply and is implicitly shared
    between copies of the same QIviPendingReply instance. At the same time the watcher provides
    signals for when a result is ready or an error happened.

    A QIviPendingReplyWatcher cannot be instantiated on its own. It is always created from a
    QIviPendingReply internally.
*/

/*!
    \class QIviPendingReplyBase
    \inmodule QtIviCore
    \brief The QIviPendingReplyBase is the base class for QIviPendingReply.

    QIviPendingReplyBase is the base class for QIviPendingReply and provides QVariant based
    functions and properties for the usage from QML.

    Usually you don't have to use this class, but instead always use the typesafe QIviPendingReply
    template class.
*/

/*!
    \qmltype PendingReply
    \qmlabstract
    \instantiates QIviPendingReply
    \inqmlmodule QtIvi

    \brief An object representing asynchronous results.

    A PendingReply is a way for providing asynchronous results. It can be used as a
    return value for asynchronous functions.

    The QML API is very similar to
    \l{https://developers.google.com/web/fundamentals/primers/promises}{JavaScript Promises}.

    This documentation shows how to use the PendingReply from within QML and how to execute code
    once the asynchronous result is ready.

    \note It is not supported to create a PendingReply from QML. The object is supposed to be
    created from C++ and returned to QML as a result. For more information on how to use it from
    C++ see the \l {QIviPendingReply}{QIviPendingReply documentation}.

    When a PendingReply is created from C++ it doesn't have a result set yet and the
    \l{PendingReply::}{resultAvailable} property is \c false. A result for the pending reply can
    only be set once and it either indicates a failed(setFailed) or a successful(setSuccess) call.
    This can be checked using the \l{PendingReply::}{success} property. The actual result is
    available from the \l{PendingReply::}{value} property, which returns undefined if no result is
    available or the reply failed.

    \section1 Using a PendingReply

    As explained above, the PendingReply is supposed to be used as a return value for asynchronous
    operations done in C++. To inform about when a reply result is available there are two ways:

    \section2 The \e then method

    Similar to a JavaScript Promise the PendingReply is then-able, which means it provides a \e then
    method. This method can be used to add callbacks which are executed when the reply succeeds or
    fails.

    \qml
    import QtQuick 2.0
    import QtIvi 1.0

    Text {
        id: root
        text: "not ready"

        Component.onCompleted: {
            var asyncReply = TestObject.asyncFunction();
            asyncReply.then(function(value) {
                                root.text = "reply ready: " + value
                            },
                            function() {
                                root.text = "reply failed"
                            })
        }
    }
    \endqml

    This simple QML snippet calls the C++ function TestObject::asyncFunction() which returns a
    PendingReply. See the \l{QIviPendingReply}{C++ part} on how to write such a function and use
    the PendingReply from C++.

    The then method is used to register two callbacks. The first callback is the result callback
    and takes the reply value as an argument. This will update the text element accordingly. The
    second argument is the failed callback, which doesn't take an argument as there is no valid
    reply value.

    If the failed state is not of interest it is also possible to not add any callback for it e.g.
    \qml
    asyncReply.then(function(value) {
                        root.text = "reply ready: " + value
                    })
    \endqml

    In a similar way only the failed callback can be registered by passing \e undefined to the
    function as the first argument:

    \qml
    asyncReply.then(undefined,
                    function() {
                        root.text = "reply failed"
                    })
    \endqml

    \section3 Synchronous results

    When a PendingReply object is used in an API the corresponding function cannot provide the
    result immediately. But especially for input validation the function can return an error state
    right away. For this the PendingReply object offers the properties
    \l{PendingReply::}{resultAvailable} and \l{PendingReply::}{success} to check for this when the
    object is given for QML.

    Checking this for every PendingReply use in QML can be tedious and produces a lot of
    boiler-plate code. Because of this the PendingReply works similar to a JavaScript Promise and
    will execute the callbacks although the result is already available.

    \section2 Signals and Slots

    Although the then method is the recommended way from QML, the PendingReply also provides
    signals. To make the PendingReply as lightweight as possible it is using Q_GADGET and cannot
    provide signals directly, but provides it through the QIviPendingReplyWatcher class. The
    QIviPendingReplyWatcher can be accessed using the \l{PendingReply::}{watcher} property.

    \note The QIviPendingReplyWatcher is owned by the PendingReply. Saving the watcher outside of
    of the PendingReply is not safe as it is destroyed once all copies of this PendingReply object
    are destroyed.
*/

/*!
    \class QIviPendingReply
    \inmodule QtIviCore
    \brief Template class for providing asynchronous results.

    A QIviPendingReply is a template class for providing asynchronous results. It can be used as a
    return value for asynchronous functions, similar to QFuture.

    In contrast to QFuture, QIviPendingReply works also in QML and is especially made for this. The
    data stored in a QIviPendingReply is implicitly shared between all copies of this reply object.
    This keeps the memory and performance footprint low.

    The QML API is very similar to
    \l{https://developers.google.com/web/fundamentals/primers/promises}{JavaScript Promises}, at
    the same time the C++ API provides support for Qt's signals and slots.

    The QIviPendingReply holds a result of a specific type. The type needs to have a default
    constructor and a copy constructor. By default the most Qt basic types are supported. New types
    can be added by using the qIviRegisterPendingReplyType function.

    When a QIviPendingReply is created it does not have a valid result set yet. This can be checked
    by using the resultAvailable property. A result for a reply can be set by using the setFailed
    or setSuccess functions. Setting the result with this function can only be done once and cannot
    be changed later. Whether a QIviPendingReply has succeeded can be determined by the success
    property.

    \section1 Writing a function returning a QIviPendingReply

    When writing a function returning a QIviPendingReply, it is often needed to do some input
    validation and return before actual doing something. Without using a QIviPendingReply one would
    write a function as follows:

    \code
    QString displayName(const QUuid &id)
    {
        if (id.isNull)
            return QString();

        //do something and wait until the result is ready (synchronous)
        asyncAPI.getDisplayName(id);
        asyncAPI.waitForFinished(&displayNameChanged);
        return asyncAPI.displayName();
    }
    \endcode

    This function is using an asynchronous API e.g. provided by an IPC. getDisplayName(id) starts
    the task and once a result is ready the displayNameChanged signal is emitted and the actual
    value can be read using the displayName() function. The provided function is using a
    waitForFinished() method to actual wait for the signal to be emitted and return the value and
    make this API synchronous.

    When moving this code to using QIviPendingReply the validation check needs to be fixed to
    return a valid QIviPendingReply. To make it more convenient to return a failed reply, the
    QIviPendingReply::createFailedReply() function be used.

    Rewriting the above function to be fully asynchronous using a QIviPendingReply it would look
    like this:

    \code
    QIviPendingReply<QString> displayName(const QUuid &id)
    {
        if (id.isNull)
            return QIviPendingReply<QString>::createFailedReply();

        QIviPendingReply<QString> reply
        //connect to the change signal and set the result to the async reply when ready
        connect(asyncAPI, &displayNameChanged, this, [reply, asyncAPI]() mutable {
                reply.setSuccess(asyncAPI.displayName());
        });
        //start getting the name
        asyncAPI.getDisplayName(id);
        return reply;
    }
    \endcode

    Now a new QIviPendingReply is created right away and passed to the lamda used in the connect
    statement. The actual task is started afterwards and the reply object is returned. Once the
    async API emits the displayNameChanged signal the lamda is executed the QIviPendingReply is
    marked as successful and the value set to the displayName().

    \note All copies of a QIviPendingReply use implicit sharing. This data is freed once all copies
    of the pending replies are deleted.

    \section1 Using functions returning a QIviPendingReply

    When using a function which returns a QIviPendingReply, the first thing to do is to check
    whether a result is already available using the isResultAvailable property and act accordingly.
    Afterwards you can start to connect the signals provided by the QIviPendingReplyWatcher.

    \section2 Signals and Slots

    In order to keep the memory footprint low, the QIviPendingReply doesn't provide signals
    directly, as it doesn't need to derive from QObject, but uses the Q_GADGET macro instead. To
    get notified once a result is ready, the QIviPendingReplyWatcher can be used instead. The
    watcher can be retrieved using the watcher property.

    Here an example on how this would work when using the API described above:

    \code
    QUuid uuid = createUuid();
    QIviPendingReply<QString> reply = displayName(uuid);
    if (reply.isResultAvailable()) {
        if (reply.isSuccessfull())
            useDisplayName(reply.value());
        else
            qWarning("getting the displayName failed");
    } else {
        connect(reply.watcher(), &QIviPendingReplyWatcher::valueChanged, this, [this, reply]() {
            if (reply.isSuccessfull())
                useDisplayName(reply.value());
            else
                qWarning("getting the displayName failed");
        });
    }
    \endcode

    As described above, the pending reply is checked first for whether a result is already available
    and if not, the signals from the watcher are used to react to the valueChanged signal.

    \note The QIviPendingReplyWatcher returned is owned by the QIviPendingReply and all its
    copies. If all copies of the QIviPendingReply get deleted its QIviPendingReplyWatcher gets
    deleted as well.

    For usage in QML see the QML documentation.
*/

QIviPendingReplyWatcher::QIviPendingReplyWatcher(int userType)
    : QObject(*new QIviPendingReplyWatcherPrivate(userType, this))
{
}

/*!
    \property QIviPendingReplyWatcher::value
    \brief Holds the current value of the QIviPendingReply

    If no result is available yet or the reply failed, a default constructed QVariant() is returned.
    Otherwise a QVariant holding the result is returned.
*/
QVariant QIviPendingReplyWatcher::value() const
{
    Q_D(const QIviPendingReplyWatcher);
    return d->m_data;
}

/*!
    \property QIviPendingReplyWatcher::valid
    \brief Holds whether the watcher is valid

    A watcher can be invalid if a QIviPendingReplyBase is manually created not using the template
    class QIviPendingReply.
*/
bool QIviPendingReplyWatcher::isValid() const
{
    Q_D(const QIviPendingReplyWatcher);
    return d->m_type != -1;
}

/*!
    \property QIviPendingReplyWatcher::resultAvailable
    \brief Holds whether a result has been set

    This property is \c true once a result has been set by using setSuccess() or setFailed().
*/
bool QIviPendingReplyWatcher::isResultAvailable() const
{
    Q_D(const QIviPendingReplyWatcher);
    return d->m_resultAvailable;
}

/*!
    \property QIviPendingReplyWatcher::success
    \brief Holds whether the reply succeeded

    This property is \c true if the reply has a valid result set by calling setSuccess().
*/
bool QIviPendingReplyWatcher::isSuccessful() const
{
    Q_D(const QIviPendingReplyWatcher);
    return d->m_success;
}

/*!
    Sets the result of the reply to \a value and marks the reply as succeeded.

    The given value needs to be of the same type than the reply or convertible to that type.

    \note a result can only be set once and cannot be changed again later.

    \sa setFailed
*/
void QIviPendingReplyWatcher::setSuccess(const QVariant &value)
{
    Q_D(QIviPendingReplyWatcher);

    if (d->m_resultAvailable) {
        qtivi_qmlOrCppWarning(this, "Result is already set. Ignoring request");
        return;
    }

    //no type checking needed when we expect a QVariant or void
    if (d->m_type == qMetaTypeId<QVariant>() || d->m_type == qMetaTypeId<void>()) {
        d->setSuccess(value);
        return;
    }

    QVariant var = value;

    //Try to convert the value, if successfully, use the converted value
    QVariant temp(var);
    if (temp.convert(d->m_type))
        var = temp;

    //We need a special conversion for enums from QML as they are saved as int
    QMetaType metaType(d->m_type);
    bool isEnumOrFlag = false;

    const QMetaObject *mo = metaType.metaObject();
    const QString enumName = QString::fromLocal8Bit(QMetaType::typeName(d->m_type)).split(QStringLiteral("::")).last();
    if (mo) {
        QMetaEnum mEnum = mo->enumerator(mo->indexOfEnumerator(enumName.toLocal8Bit().constData()));
        if (mEnum.isValid()) {
            isEnumOrFlag = true;
            if (!mEnum.isFlag() && !mEnum.valueToKey(var.toInt())) {
                qtivi_qmlOrCppWarning(this, "Enum value out of range");
                return;
            }
        }
    }

    //Check that the type names match only if it's not a enum, as it will be converted automatically in this case.
    if (!isEnumOrFlag && var.typeName() != QVariant::typeToName(d->m_type)) {
        qtivi_qmlOrCppWarning(this, QString(QStringLiteral("Expected: %1 but got %2")).arg(QLatin1String(QVariant::typeToName(d->m_type)), QLatin1String(QVariant::typeToName(value.userType()))));
        return;
    }

    d->setSuccess(var);
}

/*!
    Marks the reply as failed.

    \note a result can only be set once and cannot be changed again later.

    \sa setSuccess
*/
void QIviPendingReplyWatcher::setFailed()
{
    Q_D(QIviPendingReplyWatcher);
    if (d->m_resultAvailable) {
        qWarning("Result is already set. Ignoring request");
        return;
    }

    d->m_resultAvailable = true;
    //emitting valueChanged is intended here as it makes it easier to react to successful and failed
    //replies in the same slot.
    Q_EMIT valueChanged(d->m_data);
    Q_EMIT replyFailed();

    d->callFailedCallback();
}

/*!
    Sets the JavaScript callbacks to be called once a result is delivered. If the reply succeeded
    the \a success callback is called, otherwise the \a failed callback.

    The \a success callback can take the reply value as an argument.

    The provided values need to be \l {QJSValue::isCallable}{callable} and constructed from a
    QJSEngine. Passing QJSValue objects created by C++ will result in an error.

    Calling this function multiple times will override the existing callbacks.
*/
void QIviPendingReplyWatcher::then(const QJSValue &success, const QJSValue &failed)
{
    if (!success.isUndefined() && !success.isCallable()) {
        qtivi_qmlOrCppWarning(this, "The success functor is not callable");
        return;
    }

    if (!failed.isUndefined() && !failed.isCallable()) {
        qtivi_qmlOrCppWarning(this, "The failed functor is not callable");
        return;
    }

    Q_D(QIviPendingReplyWatcher);
    d->m_successFunctor = success;
    d->m_failedFunctor = failed;
    d->m_callbackEngine = QJSValuePrivate::engine(&d->m_successFunctor)->jsEngine();
    if (!d->m_callbackEngine)
        d->m_callbackEngine = QJSValuePrivate::engine(&d->m_failedFunctor)->jsEngine();

    if (!d->m_callbackEngine)
        qtivi_qmlOrCppWarning(this, "Couldn't access the current QJSEngine. The given callbacks will not be called without a valid QJSEngine");

    if (d->m_resultAvailable) {
        if (d->m_success)
            d->callSuccessCallback();
        else
            d->callFailedCallback();
    }
}

QIviPendingReplyBase::QIviPendingReplyBase(int userType)
    : m_watcher(new QIviPendingReplyWatcher(userType))
{
    qiviRegisterPendingReplyBasicTypes();
}

QIviPendingReplyBase::QIviPendingReplyBase(const QIviPendingReplyBase &other)
{
    this->m_watcher = other.m_watcher;
}

/*!
    \qmlproperty QIviPendingReplyWatcher* PendingReply::watcher
    \brief Holds the watcher for the PendingReply

    \note The QIviPendingReplyWatcher returned is owned by the PendingReply and all its copies. If
    all copies of the PendingReply get deleted its QIviPendingReplyWatcher gets deleted as well.
 */
/*!
    \property QIviPendingReplyBase::watcher
    \brief Holds the watcher for the QIviPendingReply

    \note The QIviPendingReplyWatcher returned is owned by the QIviPendingReply and all its
    copies. If all copies of the QIviPendingReply get deleted its QIviPendingReplyWatcher gets
    deleted as well.
 */
QIviPendingReplyWatcher *QIviPendingReplyBase::watcher() const
{
    return m_watcher.data();
}

/*!
    \qmlproperty var PendingReply::value
    \brief Holds the current value of the PendingReply

    If no result is available yet or the reply failed, a default constructed QVariant() is returned.
    Otherwise a QVariant holding the result is returned.
 */
/*!
    \property QIviPendingReplyBase::value
    \brief Holds the current value of the QIviPendingReply

    If no result is available yet or the reply failed, a default constructed QVariant() is returned.
    Otherwise a QVariant holding the result is returned.
*/
QVariant QIviPendingReplyBase::value() const
{
    if (m_watcher)
        return m_watcher->value();
    return QVariant();
}

/*!
    \qmlproperty bool PendingReply::valid
    \brief Holds whether the PendingReply is valid

    A watcher can be invalid if a PendingReply is manually created not using the template
    class QIviPendingReply.
*/
/*!
    \property QIviPendingReplyBase::valid
    \brief Holds whether the QIviPendingReplyBase is valid

    A watcher can be invalid if a QIviPendingReplyBase is manually created not using the template
    class QIviPendingReply.
*/
bool QIviPendingReplyBase::isValid() const
{
    if (m_watcher)
        return m_watcher->isValid();
    return false;
}

/*!
    \qmlproperty bool PendingReply::resultAvailable
    \brief Holds whether a result has been set

    This property is \c true once a result has been set by using setSuccess() or setFailed().
*/
/*!
    \property QIviPendingReplyBase::resultAvailable
    \brief Holds whether a result has been set

    This property is \c true once a result has been set by using setSuccess() or setFailed().
*/
bool QIviPendingReplyBase::isResultAvailable() const
{
    if (m_watcher)
        return m_watcher->isResultAvailable();
    return false;
}

/*!
    \qmlproperty bool PendingReply::success
    \brief Holds whether the reply succeeded

    This property is \c true if the reply has a valid result set by calling setSuccess().
*/
/*!
    \property QIviPendingReplyBase::success
    \brief Holds whether the reply succeeded

    This property is \c true if the reply has a valid result set by calling setSuccess().
*/
bool QIviPendingReplyBase::isSuccessful() const
{
    if (m_watcher)
        return m_watcher->isSuccessful();
    return false;
}

/*!
    \qmlmethod PendingReply::then(success, failed)

    Sets the JavaScript callbacks to be called once a result is delivered. If the reply succeeded
    the \a success callback is called, otherwise the \a failed callback.

    The \a success callback can take the reply value as an argument.

    See \l{PendingReply#The then method}{The \e then method} for example usage.

    Calling this function multiple times will override the existing callbacks.
*/
/*!
    Sets the JavaScript callbacks to be called once a result is delivered. If the reply succeeded
    the \a success callback is called, otherwise the \a failed callback.

    The \a success callback can take the reply value as an argument.

    The provided values need to be \l {QJSValue::isCallable}{callable} and constructed from a
    QJSEngine. Passing QJSValue objects created by C++ will result in an error.

    Calling this function multiple times will override the existing callbacks.
*/
void QIviPendingReplyBase::then(const QJSValue &success, const QJSValue &failed)
{
    if (m_watcher)
        m_watcher->then(success, failed);
}

/*!
    \qmlmethod PendingReply::setSuccess(var value)

    Sets the result of the reply to \a value and marks the reply as succeeded.

    The given value needs to be of the same type as the reply or be convertible to that type.

    \note a result can only be set once and cannot be changed again later.

    \sa setFailed
*/
/*!
    Sets the result of the reply to \a value and marks the reply as succeeded.

    The given value needs to be of the same type as the reply or be convertible to that type.

    \note a result can only be set once and cannot be changed again later.

    \sa setFailed
*/
void QIviPendingReplyBase::setSuccess(const QVariant &value)
{
    if (m_watcher)
        m_watcher->setSuccess(value);
}

/*!
    \qmlmethod PendingReply::setFailed()

    Marks the reply as failed.

    \note a result can only be set once and cannot be changed again later.

    \sa setSuccess
*/
/*!
    Marks the reply as failed.

    \note a result can only be set once and cannot be changed again later.

    \sa setSuccess
*/
void QIviPendingReplyBase::setFailed()
{
    if (m_watcher)
        m_watcher->setFailed();
}

/*!
    \internal

    Sets the result of the reply to \a value and marks the reply as succeeded, but without checking
    if the QVariant can be converted. This is used by the template class as we convert it to a
    QVariant before anyway and can be sure the type is correct.
*/
void QIviPendingReplyBase::setSuccessNoCheck(const QVariant &value)
{
    if (m_watcher)
        m_watcher->d_func()->setSuccess(value);
}

/*!
    \fn QIviPendingReplyWatcher::replyFailed()

    Emitted when the reply is marked as failed.

    \sa setFailed
*/

/*!
    \fn QIviPendingReplyWatcher::replySuccess()

    Emitted when the reply is marked as successful.

    \sa setSuccess
*/

/*!
    \fn QIviPendingReplyWatcher::valueChanged(const QVariant &value)

    Emitted when the result for the reply is ready. This signal is called when the reply is
    successful as well as when it is failed. The \a value argument holds the result and is a default
    constructed QVariant in the failed case.

    \sa setSuccess setFailed
*/

/*!
    \fn template <class T> QIviPendingReply<T>::createFailedReply()

    Creates a reply object which is marked as failed. This is convenient in error cases inside
    functions returning a reply e.g.

    \code
    QIviPendingReply<QString> doSomething(int value)
    {
        if (value <= 0) {
            qWarning("The value needs to be bigger than 0");
            return QIviPendingReply<QString>::createFailedReply()
        }

        QIviPendingReply<QString> reply;
        ...
        return reply;
    }
    \endcode
*/

/*!
    \fn template <class T> QIviPendingReply<T>::QIviPendingReply(const T &value)

    Creates a new QIviPendingReply that stores type T.
    The pending reply is set to successful using \a value.

    This is equivalent to:
    \code
    QIviPendingReply<T> reply.
    reply.setSuccess(value);
    \endcode
*/

/*!
    \fn template <class T> QIviPendingReply<T>::reply() const

    Returns the result of the reply. If no result has been set yet or when the reply is marked as
    failed, a default constructed value is returned.

    \sa setSuccess setFailed
*/

/*!
    \fn template <class T> QIviPendingReply<T>::setSuccess(const T &val)

    Sets the result of the reply to \a val and marks the reply as succeeded.

    \note a result can only be set once and cannot be changed again later.

    \sa setFailed
*/

/*!
    \fn template <class T> QIviPendingReply<T>::then(const std::function<void (const T &)> &success, const std::function<void ()> &failed)

    Sets the C++ callbacks to be called once a result is delivered. If the reply succeeds
    \a success is called; otherwise \a failed is called.

    The \a success callback gets the reply value as an argument.

    In case the result of the pending reply is already available when this function is called, the corresponding callback functions are
    run immediately.

    \sa QIviPendingReplyBase::then
*/

/*!
    \fn qIviRegisterPendingReplyType(const char *name)
    \relates QIviPendingReply

    Registers the type name \a name for the type \c{T} for usage inside a QIviPendingReply. Any
    class or struct that has a public default constructor, a public copy constructor and a public
    destructor can be registered.

    This function requires that \c{T} is a fully defined type at the point where the function is
    called. For pointer types, it also requires that the pointed-to type is fully defined. Use
    Q_DECLARE_OPAQUE_POINTER() to be able to register pointers to forward declared types.

    Please see qRegisterMetaType for more information.
*/

QT_END_NAMESPACE
