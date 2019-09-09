#ifndef ONBPACKET_H
#define ONBPACKET_H

#include "xotools_global.h"
#include "ONBCommon.h"
#include <QByteArray>
#include <QDebug>

enum format_data
{
    T_NONE,
    T_INTEGER,
    T_INTEGER_ARR,
    T_FLOAT,
    T_FLOAT_ARR,
    T_DOUBLE,
    T_DOUBLE_ARR,
    T_TEXT,
    T_IMAGE
};

class XOTOOLSSHARED_EXPORT ONBPacket
{
private:
    ONBHeader m_header;
    QByteArray m_data;

    format_data m_format_data;

public:
    explicit ONBPacket(const ONBHeader &header, const QByteArray &data = QByteArray(), format_data in_type = T_NONE);
    explicit ONBPacket(const QByteArray &data);
    explicit ONBPacket() { }
//    ~ONBPacket() {qDebug() << "packet deleted" << this;}

    const ONBHeader &header() const {return m_header;}
    const QByteArray &data() const {return m_data;}

    format_data formatData() const;

    void writePacket(QByteArray &data) const;
};

#endif // ONBPACKET_H
