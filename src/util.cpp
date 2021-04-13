#include "util.h"

void runOnMainThread(std::function<void ()> f)
{
    QCoreApplication::postEvent(Executor::instance(), new ExecuteEvent({f}));
}

Executor* Executor::instance()
{
    static Executor* s = new Executor;
    return s;
}

void Executor::customEvent(QEvent* ev)
{
    if (auto v = dynamic_cast<ExecuteEvent*>(ev))
    {
        v->data();
    }
}
