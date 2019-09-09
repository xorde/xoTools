#include "ObjectInfo.h"
#include "onbobject.h"

#include <QDebug>

ObjectInfo::ObjectInfo() :
    mReadPtr(nullptr),
    mWritePtr(nullptr)
{
    //        qDebug() << "ObjectInfo created" << this;
}

ObjectInfo::ObjectInfo(const ObjectDescription &desc) :
    mReadPtr(nullptr),
    mWritePtr(nullptr),
    mDesc(desc)
{
//    qDebug() << "ObjectInfo created with description" << this;
}

QJsonObject ObjectInfo::createJsonInfo() const
{
    QJsonObject obj;
    obj["name"] = mDesc.name;
    obj["flag"] = mDesc.flags == Input ? "Input" : "Output";
    obj["type"] = QString::number(mDesc.flags == Input ? mDesc.wType : mDesc.rType);
    return obj;
}

#ifdef _MSC_VER
template<>
ONBObject<QByteArray> &ObjectInfo::create(QString name, QByteArray &var, Flags flags)
{
    ONBObject<QByteArray> *obj = new ONBObject<QByteArray>;
    unsigned short sz = 0;
    Type t = typeOfVar(var);
    if (flags & Read)
    {
        obj->mReadPtr = &var;
        obj->mDesc.readSize = sz;
        obj->mDesc.rType = t;
    }
    if (flags & Write)
    {
        obj->mWritePtr = &var;
        obj->mDesc.writeSize = sz;
        obj->mDesc.wType = t;
    }
    obj->mDesc.flags = flags;
    obj->mDesc.name = name;
    return *obj;
}
#endif

QByteArray ObjectInfo::read() const
{
    requestEvent();
    QByteArray ba;
    doRead(ba, mReadPtr);
    return ba;
}

bool ObjectInfo::doRead(QByteArray &ba, const void *ptr) const
{
    if (!ptr)
        return false;

    if (!ptr || !(mDesc.flags & Read))
        return false;
    if (mDesc.rType == String)
    {
        const QString *str = reinterpret_cast<const QString*>(ptr);
        if (str)
        {
            ba.append(str->toUtf8());
            return true;
        }
        return false;
    }
    else if (mDesc.rType == Common && mDesc.readSize == 0) // pure (Q)ByteArray
    {
        ba.append(*reinterpret_cast<const QByteArray*>(ptr));
        return true;
    }
    else
    {
        ba.append(reinterpret_cast<const char*>(ptr), mDesc.readSize);
        return true;
    }
}

bool ObjectInfo::doWrite(const QByteArray &ba, void *ptr)
{
    bool changed = false;

    if (!ptr || !(mDesc.flags & Write))
        return false;
    if (mDesc.wType == String)
    {
        QString *str = reinterpret_cast<QString*>(ptr);
        if (!str)
            return false;
        else
        {
            QString newstr = QString::fromUtf8(ba.data(), ba.size());
            if (*str != newstr)
                changed = true;
            *str = newstr;
        }
    }
    else if (mDesc.wType == Common && mDesc.writeSize == 0) // pure (Q)ByteArray
    {
        QByteArray &oldBa = *reinterpret_cast<QByteArray*>(ptr);
        if (oldBa != ba)
            changed = true;
        oldBa = ba;
    }
    else if ((size_t)ba.size() == mDesc.writeSize)
    {
        unsigned char *dst = reinterpret_cast<unsigned char*>(ptr);
        changed = memcmp(dst, ba.data(), ba.size());
        memcpy(dst, ba.data(), ba.size());
    }
    else
    {
        return false;
    }

    if (ptr == mWritePtr)
    {
        this->m_changed |= changed;
        receiveEvent();
        if (this->m_changed)
            valueChangeEvent();
    }
    return true;
}

bool ObjectInfo::read(QByteArray &ba) const
{
    return doRead(ba, mReadPtr);
}

bool ObjectInfo::write(const QByteArray &ba)
{
    return doWrite(ba, mWritePtr);
}

int ObjectInfo::sizeofType(ObjectInfo::Type type) const
{
    switch (type)
    {
    case Void:      return 0;
    case Bool:      return 1;
        case Int:       return 4;
        case UInt:      return 4;
        case LongLong:  return 8;
        case ULongLong: return 8;
        case Double:    return 8;
        case Long:      return 4;
        case Short:     return 2;
        case Char:      return 1;
        case ULong:     return 4;
        case UShort:    return 2;
        case UChar:     return 1;
        case Float:     return 4;
        case SChar:     return 1;
        default:        return 0;
    }
}
