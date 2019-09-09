#include "objectdescription.h"

#include <QDebug>

ObjectDescriptionHeaderV2::ObjectDescriptionHeaderV2()
{
    memset(this, 0, sizeof(ObjectDescriptionHeaderV2));
    version = 2;
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
    ba.append(reinterpret_cast<const char*>(this), sizeof(ObjectDescriptionHeaderV2));
    ba.append(name.toUtf8());
}

bool ObjectDescription::parse(const QByteArray &ba)
{
    if ((size_t)ba.size() < sizeof(ObjectDescriptionHeaderV1))
        return false;
    const ObjectDescriptionHeaderV2 *hdr = reinterpret_cast<const ObjectDescriptionHeaderV2*>(ba.constData());
    if (hdr->_verid == 0xF) // new-style descriptor
    {
        if (hdr->version == 2)
        {
            id = hdr->id;
            group = hdr->group;
            // swap read/write info
            flags = swapRWflags(hdr->flags);
            extFlags = hdr->extFlags;
            rType = hdr->wType;
            wType = hdr->rType;
            readSize = hdr->writeSize;
            writeSize = hdr->readSize;
            const char *namedata = reinterpret_cast<const char*>(hdr + 1);
            name = QString::fromUtf8(namedata, ba.size() - sizeof(ObjectDescriptionHeaderV2));
            return true;
        }
    }
    else // old-style descriptor (version 1)
    {
        const ObjectDescriptionHeaderV1 *hdr = reinterpret_cast<const ObjectDescriptionHeaderV1*>(ba.constData());
        id = hdr->id;
        // swap read/write info
        flags = swapRWflags(hdr->flags);
        rType = hdr->wType;
        wType = hdr->rType;
        readSize = hdr->writeSize;
        writeSize = hdr->readSize;
        const char *namedata = reinterpret_cast<const char*>(hdr + 1);
        name = QString::fromUtf8(namedata, ba.size() - sizeof(ObjectDescriptionHeaderV1));
        return true;
    }
    return false;
}
