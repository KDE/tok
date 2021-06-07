#pragma once

#include <QtQml>
#include <QObject>

class CopyInterceptor : public QObject
{
    Q_OBJECT

    class Private;
    std::unique_ptr<Private> d;

    Q_PROPERTY(QJSValue copy READ copy WRITE setCopy)
    Q_PROPERTY(QJSValue paste READ paste WRITE setPaste)

public:
    explicit CopyInterceptor(QObject* parent = nullptr);
    ~CopyInterceptor();

    QJSValue copy();
    QJSValue paste();
    void setCopy(QJSValue val);
    void setPaste(QJSValue val);

    Q_INVOKABLE void copyText(const QString& txt);

    bool eventFilter(QObject *object, QEvent *event);

    static CopyInterceptor *qmlAttachedProperties(QObject* on);
};

QML_DECLARE_TYPEINFO(CopyInterceptor, QML_HAS_ATTACHED_PROPERTIES)
