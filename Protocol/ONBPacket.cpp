#include "ONBPacket.h"

ONBPacket::ONBPacket(const ONBHeader &header, const QByteArray &data, format_data in_type)
    : m_header(header), m_data(data), m_format_data(in_type)
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

format_data ONBPacket::formatData() const
{
    return m_format_data;
}
