#pragma once

#include <QString>
#include <QCoreApplication>

class ExecuteEvent : public QEvent {
public:
    static constexpr int typeID = QEvent::User + 1;
    ExecuteEvent(std::function<void()> in) : QEvent(QEvent::Type(typeID)), data(in) {}
    std::function<void()> data;
};

class Executor : public QObject
{
    Q_OBJECT

    void customEvent(QEvent *event) override;

public:
    static Executor* instance();
};

void runOnMainThread(std::function<void ()> f);
