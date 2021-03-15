#include <cstring>

#include "qquickrelationallistener_p.h"

QQmlRelationalListener::QQmlRelationalListener(QObject* parent)
    : QObject(parent)
    , QQmlParserStatus()
    , d_ptr(new QQmlRelationalListenerPrivate(this))
{
}

QQmlRelationalListener::~QQmlRelationalListener()
{

}

void QQmlRelationalListener::componentComplete()
{
    Q_ASSERT(!d_ptr->shape.isNull());
    Q_ASSERT(d_ptr->shape->isReady());

    connect(d_ptr->relationalModel, &QAbstractRelationalModel::keyDataChanged, this, [this](const QVariant& key, const QVector<int>& roles) {
        if (d_ptr->key != key) {
            return;
        }

        applyChanged(roles);
    });

    checkKey();
    applyChanged({});
    d_ptr->complete = true;
}

void QQmlRelationalListener::checkKey()
{
    if (!d_ptr->relationalModel->checkKey(d_ptr->key)) {
        if (d_ptr->relationalModel->canFetchKey(d_ptr->key)) {
            d_ptr->relationalModel->fetchKey(d_ptr->key);
        }
    } else {
        applyChanged({});
    }
}

void QQmlRelationalListener::applyChanged(const QVector<int>& roles)
{
    const auto roleNames = d_ptr->relationalModel->roleNames();
    QHash<QByteArray,int> invertedRoleNames;
    for (auto it : roleNames.keys()) {
        invertedRoleNames[roleNames[it]] = it;
    }

    QVariantMap props;

    auto on = d_ptr->dataObject;
    auto fromScratch = on == nullptr;
    if (fromScratch) {
        if (d_ptr->shape->status() == QQmlComponent::Error) {
            qFatal("%s", d_ptr->shape->errorString().toStdString().c_str());
        }
        on = d_ptr->shape->beginCreate(qmlContext(this));
    }
    auto mo = on->metaObject();

    for (int i = mo->propertyOffset(); i < mo->propertyCount(); i++) {
        QByteArray propName = mo->property(i).name();

        if (!invertedRoleNames.contains(propName)) {
            qWarning() << "Model doesn't contain" << propName.toStdString().c_str();
            continue;
        }

        auto role = invertedRoleNames[propName];
        if (!roles.contains(role) && !roles.isEmpty()) {
            continue;
        }

        if (!fromScratch) {
            mo->property(i).write(d_ptr->dataObject, d_ptr->relationalModel->data(d_ptr->key, role));
        }
    }

    if (fromScratch) {
        d_ptr->shape->setInitialProperties(on, props);
        d_ptr->shape->completeCreate();
        d_ptr->dataObject = on;
        Q_EMIT dataChanged();
    }
}

QAbstractRelationalModel* QQmlRelationalListener::model() const
{
    return d_ptr->relationalModel;
}

void QQmlRelationalListener::setModel(QAbstractRelationalModel* setModel)
{
    if (setModel == d_ptr->relationalModel) {
        return;
    }

    d_ptr->relationalModel = setModel;
    Q_EMIT modelChanged();
}

void QQmlRelationalListener::resetModel()
{
    setModel(nullptr);
}

QVariant QQmlRelationalListener::key() const
{
    return d_ptr->key;
}

void QQmlRelationalListener::setKey(const QVariant& key)
{
    if (key == d_ptr->key) {
        return;
    }

    d_ptr->key = key;
    Q_EMIT keyChanged();
    if (d_ptr->complete) {
        checkKey();
    }
}

void QQmlRelationalListener::resetKey()
{
    setKey(QVariant());
}

QQmlComponent* QQmlRelationalListener::shape() const
{
    return d_ptr->shape;
}

void QQmlRelationalListener::setShape(QQmlComponent* shape)
{
    if (shape == d_ptr->shape) {
        return;
    }

    d_ptr->shape = shape;
    Q_EMIT shapeChanged();
}

QObject* QQmlRelationalListener::data() const
{
    return d_ptr->dataObject;
}
