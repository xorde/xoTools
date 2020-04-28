#ifndef ONBPACKET_H
#define ONBPACKET_H

#include "xotools_global.h"
#include "ONBCommon.h"
#include <QByteArray>
#include <QDebug>

class XOTOOLSSHARED_EXPORT ONBPacket
{
private:
    ONBHeader m_header;
    QByteArray m_data;

public:
    explicit ONBPacket(const ONBHeader &header, const QByteArray &data = QByteArray());
    explicit ONBPacket(const QByteArray &data);
    explicit ONBPacket() { }
//    ~ONBPacket() {qDebug() << "packet deleted" << this;}

    const ONBHeader &header() const {return m_header;}
    const QByteArray &data() const {return m_data;}

    void writePacket(QByteArray &data) const;
};

#endif // ONBPACKET_H
