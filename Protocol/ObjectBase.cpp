#include "ObjectBase.h"
#include "ComponentBase.h"

ObjectBase::ObjectBase() :
    id(m_description.id), RMIP(m_RMIP), timestamp(m_timestamp),
    m_changed(false), m_needTimestamp(false), m_RMIP(0), m_autoPeriodMs(0), m_timestamp(0),
    m_component(nullptr) // DIRTY HACK
{

}

ObjectBase::~ObjectBase()
{

}

bool ObjectBase::read(QByteArray &ba)
{
    return readMeta(ba, MV_Value);
}

bool ObjectBase::write(const QByteArray &ba)
{
    bool result = writeMeta(ba, MV_Value);
    return result;
}

bool ObjectBase::testExtFlag(ObjectBase::MetaValue meta) const
{
    if (meta == MV_Value)
        return true;
    return m_description.extFlags & (1 << (meta - 1));
}

QString ObjectBase::typeName() const
{
    return QVariant::typeToName(m_description.type);
}

QByteArray ObjectBase::read()
{
    QByteArray ba;
    readMeta(ba, MV_Value);
    return ba;
}

void ObjectBase::setEvent()
{
    if (m_component)
        m_component->touchOutput(name()); // DIRTY HACK
}

void ObjectBase::setExtFlag(ObjectBase::MetaValue meta)
{
    m_description.extFlags |= (1 << (meta - 1));
}

void ObjectBase::setId(unsigned char id, unsigned char group)
{
    m_description.id = id;
    m_description.group = group;
}

QJsonObject ObjectBase::createJsonInfo() const
{
    QJsonObject obj;
    obj["name"] = name();
    obj["flag"] = flags() == Input ? "Input" : "Output"; //! @todo: there is XEPb, govnoflags in ObjectBase JSON description!!
    obj["type"] = QString::number(m_description.type);
    return obj;
}
