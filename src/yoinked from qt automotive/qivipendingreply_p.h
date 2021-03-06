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

#ifndef QIVIPENDINGREPLY_P_H
#define QIVIPENDINGREPLY_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <private/qobject_p.h>

#include "qivipendingreply.h"

QT_BEGIN_NAMESPACE

class Q_QTIVICORE_EXPORT QIviPendingReplyWatcherPrivate : public QObjectPrivate
{
public:
    QIviPendingReplyWatcherPrivate(int userType, QIviPendingReplyWatcher *parent);

    void setSuccess(const QVariant &value);
    void callSuccessCallback();
    void callFailedCallback();

    QIviPendingReplyWatcher * const q_ptr;
    Q_DECLARE_PUBLIC(QIviPendingReplyWatcher)
    Q_DISABLE_COPY(QIviPendingReplyWatcherPrivate)

    int m_type;
    bool m_resultAvailable;
    bool m_success;
    QVariant m_data;
    QJSValue m_successFunctor;
    QJSValue m_failedFunctor;
    QJSEngine *m_callbackEngine;
};

QT_END_NAMESPACE

#endif // QIVIPENDINGREPLY_P_H
