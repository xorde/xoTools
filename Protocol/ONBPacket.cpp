#include "ONBPacket.h"

ONBPacket::ONBPacket(const ONBHeader &header, const QByteArray &data)
    : m_header(header), m_data(data)
{

}

ONBPacket::ONBPacket(const QByteArray &data)
{
    m_header = *reinterpret_cast<const ONBHeader*>(data.data());
    m_data = data.mid(sizeof(ONBHeader));
}

void ONBPacket::writePacket(QByteArray &data) const
{
    data.append(reinterpret_cast<const char*>(&m_header), sizeof(ONBHeader));
    data.append(m_data);
}
