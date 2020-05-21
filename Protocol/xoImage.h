#ifndef XOIMAGEW_H
#define XOIMAGEW_H

#include "xoObject.h"
#include <QBuffer>

template<>
void xoObjectBase<QImage>::fillDescription()
{
    m_description.size = 0;
    m_description.type = static_cast<Type>(QMetaType::QImage);
}

template<>
void xoObjectBase<QImage>::doRead(QByteArray &ba, const QImage *p_image)
{
    QBuffer writer(&ba);
    writer.open(QIODevice::WriteOnly);
    p_image->save(&writer, "jpg");
    writer.close();
}

template<>
bool xoObjectBase<QImage>::doWrite(const QByteArray &ba, QImage *p_image)
{
    QImage newImage = QImage::fromData(ba);
    bool changed = (*p_image != newImage);
    *p_image = newImage;
    return changed;
}

typedef xoObject<QImage> xoImage;

#endif // XOIMAGEW_H
