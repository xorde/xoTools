#include "objectdescription.h"

ObjectDescriptionHeaderV2::ObjectDescriptionHeaderV2()
{
    memset(this, 0, sizeof(ObjectDescriptionHeaderV2));
    version = 2;
    _verid = 0xF;
}

ObjectDescriptionHeaderV3::ObjectDescriptionHeaderV3()
{
    memset(this, 0, sizeof(ObjectDescriptionHeaderV3));
    version = 3;
    _verid = 0xF;
}


ObjectDescription::ObjectDescription()
{

}

ObjectDescription::ObjectDescription(const QByteArray &ba)
{
    parse(ba);
}

QString ObjectDescription::flagString() const
{
    QString res = "afdhswrv";
    for (int i=0; i<8; i++)
    {
        if (!(flags & (1<<i)))
            res[7-i] = '-';
    }
    return res;
}

void ObjectDescription::read(QByteArray &ba) const
{
    ba.append(reinterpret_cast<const char*>(this), sizeof(ObjectDescriptionHeaderV3));
    ba.append(name.toUtf8());
}

bool ObjectDescription::parse(const QByteArray &ba)
{
    if (ba.size() < static_cast<int>(sizeof(ObjectDescriptionHeaderV1)))
        return false;
    const ObjectDescriptionHeaderV3 *hdr = reinterpret_cast<const ObjectDescriptionHeaderV3*>(ba.constData());
    if (hdr->_verid == 0xF) // new-style descriptor
    {
        if (hdr->version == 3)
        {
            *static_cast<ObjectDescriptionHeaderV3*>(this) = *hdr;
            flags = swapRWflags(hdr->flags);
            const char *namedata = reinterpret_cast<const char*>(hdr + 1);
            name = QString::fromUtf8(namedata, ba.size() - static_cast<int>(sizeof(ObjectDescriptionHeaderV3)));
            return true;
        }
        else if (hdr->version == 2)
        {
            const ObjectDescriptionHeaderV2 *hdr2 = reinterpret_cast<const ObjectDescriptionHeaderV2*>(hdr);
            id = hdr2->id;
            group = hdr2->group;
            // swap read/write info
            flags = swapRWflags(hdr2->flags);
            extFlags = hdr2->extFlags;
            type = hdr2->wType? hdr2->wType: hdr2->rType;
            size = hdr2->writeSize? hdr2->writeSize: hdr2->readSize;
            const char *namedata = reinterpret_cast<const char*>(hdr2 + 1);
            name = QString::fromUtf8(namedata, ba.size() - static_cast<int>(sizeof(ObjectDescriptionHeaderV2)));
            return true;
        }
    }
    else // old-style descriptor (version 1)
    {
        const ObjectDescriptionHeaderV1 *hdr = reinterpret_cast<const ObjectDescriptionHeaderV1*>(ba.constData());
        id = hdr->id;
        // swap read/write info
        flags = swapRWflags(hdr->flags);
        type = hdr->wType? hdr->wType: hdr->rType;
        size = hdr->writeSize? hdr->writeSize: hdr->readSize;
        const char *namedata = reinterpret_cast<const char*>(hdr + 1);
        name = QString::fromUtf8(namedata, ba.size() - static_cast<int>(sizeof(ObjectDescriptionHeaderV1)));
        return true;
    }
    return false;
}
