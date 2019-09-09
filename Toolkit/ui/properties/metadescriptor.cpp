#include "metadescriptor.h"

MetaDescriptor::MetaDescriptor(QObject *parent) : AbstractMetaDescriptor(parent)
{
}

void MetaDescriptor::parse(QObject *object)
{
    m_object = object;

    const QMetaObject *metaobject = m_object->metaObject();
    int count = metaobject->propertyCount();

    static const QString objectNameString = "objectName";

    for (int i = 0; i < count; ++i)
    {
        QMetaProperty metaproperty = metaobject->property(i);

        if (objectNameString.compare(metaproperty.name(), Qt::CaseInsensitive) == 0 || !metaproperty.isWritable())
            continue;

        MetaDescription *property = new MetaDescription(this);
        property->set(i, object);

        m_properties.push_back(property);
    }
}

QString MetaDescriptor::getClassName()
{
    return m_object->metaObject()->className();
}

void MetaDescriptor::setItem(QMetaProperty *value)
{
    m_item = value;
    emit itemChanged(m_item);
}

QMetaProperty *MetaDescriptor::getItem()
{
    return m_item;
}

void MetaDescriptor::set(QObject *object)
{
    parse(object);
}


