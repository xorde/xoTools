#ifndef XOOBJECTBASE_H
#define XOOBJECTBASE_H

#include "ObjectBase.h"
#include <QImage>

template<typename T>
class xoObjectBase : public virtual ObjectBase
{
public:
    xoObjectBase() : m_ptr(&m_value)
    {
        fillDescription();
    }
    xoObjectBase(const T &copyval) : m_value(copyval), m_ptr(&m_value)
    {
        fillDescription();
    }

    void operator =(const xoObjectBase<T> &other)
    {
        *m_ptr = *other.m_ptr;
    }

    T &value() {return *m_ptr;}
    const T &value() const {return *m_ptr;}
    operator T&() {return *m_ptr;}
    operator const T&() const {return *m_ptr;}

    T *operator &() {return &m_value;}
    const T *operator&() const {return &m_value;}

    void operator =(const T &v)
    {
        *m_ptr = v;
        setEvent();
    }
//    void operator =(T v)
//    {
//        *m_ptr = v;
//        setEvent();
//    }

protected:
    T m_value;
    T *m_ptr;
    T m_min, m_max, m_def, m_step;

    friend class ComponentBase;

    virtual bool readMeta(QByteArray &ba, MetaValue meta) override final
    {
        if (!testExtFlag(meta))
            return false;
        switch (meta)
        {
          case MV_Value:
            if (m_description.flags & Read)
            {
                requestEvent();
                doRead(ba, m_ptr);
            }
            return true;
          case MV_Min: doRead(ba, &m_min); break;
          case MV_Max: doRead(ba, &m_max); break;
          case MV_Def: doRead(ba, &m_def); break;
          case MV_Step: doRead(ba, &m_step); break;
          case MV_Mime: ba.append(m_mimeType.toUtf8()); return true;
          case MV_Hint: ba.append(m_hint.toUtf8()); return true;
          case MV_Unit: ba.append(m_unit.toUtf8()); return true;
          case MV_Options: ba.append(m_options.toUtf8()); return true;
          case MV_RMIP: ba.append(reinterpret_cast<const char*>(&m_RMIP), sizeof(int32_t)); return true;
          case MV_Enum: ba.append(m_enum.join(",").toUtf8()); return true;
        }
        return false;
    }

    virtual bool writeMeta(const QByteArray &ba, MetaValue meta) override final
    {
        setExtFlag(meta);
        switch (meta)
        {
          case MV_Value:
            if (m_description.flags & Write)
            {
                m_changed |= doWrite(ba, m_ptr);
                receiveEvent();
                if (m_changed)
                    changeEvent();
            }
            break;
          case MV_Min: doWrite(ba, &m_min); break;
          case MV_Max: doWrite(ba, &m_max); break;
          case MV_Def: doWrite(ba, &m_def); break;
          case MV_Step: doWrite(ba, &m_step); break;
          case MV_Mime: m_mimeType = QString::fromUtf8(ba.constData()); break;
          case MV_Hint: m_hint = QString::fromUtf8(ba.constData()); break;
          case MV_Unit: m_unit = QString::fromUtf8(ba.constData()); break;
          case MV_Options: m_options = QString::fromUtf8(ba.constData()); break;
          case MV_RMIP: m_RMIP = *reinterpret_cast<const int32_t*>(ba.constData()); m_needTimestamp = m_RMIP > 0; break;
          case MV_Enum: m_enum = QString::fromUtf8(ba.constData()).split(","); break;
        }
        return true;
    }

    void copyInfoFrom(const xoObjectBase<T> &other)
    {
        m_description = other.m_description;
        m_changed = other.m_changed;
        m_needTimestamp = other.m_needTimestamp;
        m_RMIP = other.m_RMIP;
        m_autoPeriodMs = other.m_autoPeriodMs;
        m_timestamp = other.m_timestamp;

        m_mimeType = other.m_mimeType;
        m_hint = other.m_hint;
        m_unit = other.m_unit;
        m_options = other.m_options;
        m_enum = other.m_enum;

        m_min = other.m_min;
        m_max = other.m_max;
        m_def = other.m_def;
        m_step = other.m_step;
    }

//    Type typeId() const {return UnknownType;}

    void restoreDescription() {fillDescription();}

private:
    void doRead(QByteArray &ba, const T *ptr)
    {
        ba.append(reinterpret_cast<const char*>(ptr), m_description.size);
    }

    bool doWrite(const QByteArray &ba, T *ptr)
    {
        bool changed = false;
        int sz = m_description.size < ba.size()? m_description.size: ba.size();
        changed = memcmp(ptr, ba.data(), sz);
        if (ba.size() != m_description.size)
        {
//            qDebug() << "[doWrite]: x3 size";
            if (m_description.type == UInteger)
                *ptr = 0;
            else if (m_description.type == Integer)
                *ptr = ba.back() & 0x80? -1: 0;
            else
                return false;
        }
        memcpy(ptr, ba.data(), sz);
        return changed;
    }

    void fillDescription()
    {
        if (std::is_fundamental<T>::value)
            m_value = 0;
        else
            m_value = T();
        m_description.size = sizeof(T);
        m_description.type = Common;
        if (std::is_integral_v<T>)
        {
            if (std::is_unsigned_v<T>)
                m_description.type = UInteger;
            else
                m_description.type = Integer;
        }
    }
};

template<> void xoObjectBase<bool>::fillDescription() {m_description.size = sizeof(bool); m_description.type = Bool;}
template<> void xoObjectBase<double>::fillDescription() {m_description.size = sizeof(double); m_description.type = Double;}
template<> void xoObjectBase<float>::fillDescription() {m_description.size = sizeof(float); m_description.type = Float;}
template<> void xoObjectBase<QString>::fillDescription() {m_description.size = 0; m_description.type = String;}
template<> void xoObjectBase<QByteArray>::fillDescription() {m_description.size = 0; m_description.type = Common;}
template<> void xoObjectBase<QVariant>::fillDescription() {m_description.size = 0; m_description.type = Variant;}

template<>
void xoObjectBase<QString>::doRead(QByteArray &ba, const QString *ptr)
{
    ba.append(ptr->toUtf8());
}

template<>
bool xoObjectBase<QString>::doWrite(const QByteArray &ba, QString *ptr)
{
    QString newstr = QString::fromUtf8(ba.data(), ba.size());
    bool changed = (*ptr != newstr);
    *ptr = newstr;
    return changed;
}

template<>
void xoObjectBase<QByteArray>::doRead(QByteArray &ba, const QByteArray *ptr)
{
    ba.append(*ptr);
}

template<>
bool xoObjectBase<QByteArray>::doWrite(const QByteArray &ba, QByteArray *ptr)
{
    bool changed = (*ptr != ba);
    *ptr = ba;
    return changed;
}

template<>
void xoObjectBase<QVariant>::doRead(QByteArray &ba, const QVariant *ptr)
{
    ba.append(ptr->toString().toUtf8());
}

template<>
bool xoObjectBase<QVariant>::doWrite(const QByteArray &ba, QVariant *ptr)
{
    QVariant newvar = QString::fromUtf8(ba.data(), ba.size());
    bool changed = (*ptr != newvar);
    *ptr = newvar;
    return changed;
}

#endif // XOOBJECTBASE_H
