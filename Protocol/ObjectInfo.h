#ifndef OBJECTINFO_H
#define OBJECTINFO_H

#include "xotools_global.h"
#include "objectdescription.h"
#include <QJsonObject>
#include <QDebug>

using namespace std::chrono_literals;

template<typename T>
class xoObject;

class XOTOOLSSHARED_EXPORT ObjectBase
{
public:
    //! Access flags of the Object.
    typedef enum
    {
        Volatile= 0x01, //!< Volatile Object is input or output, changes in real-time.
        Read    = 0x02, //!< Object can be read
        Write   = 0x04, //!< Object can be written
        Save    = 0x08, //!< Object value is stored in NVM
        Hidden  = 0x10, //!< Object is hidden for users, may be considered for internal use
        //Dual    = 0x20, //!< Object is read from one location, written to another
        Function= 0x40, //!< Object is the function (now supported in devices only)
        Array   = 0x80, //!< Object is array (now supported in devices only)

        ReadOnly    = Read,                 //!< Alias for read-only access
        WriteOnly   = Write,                //!< Alias for write-only access
        ReadWrite   = Read | Write,         //!< Alias for read-write access

        Constant    = Read,                 //!< Constant object is not volatile and can only be read
        Input       = Write | Volatile,     //!< Input is volatile object that can oly be written
        Output      = Read | Volatile,      //!< Output is volatile
        IO          = ReadWrite | Volatile, //!< Input and output simultaneously
        Variable    = Read | Write,         //!< Common object with read and write access
        Setting     = Variable | Save,      //!< Object with read-write access, its value stored in NVM

        SecretVar   = ReadWrite | Hidden,   //!< Common hidden object
        SecretSetting = Setting | Hidden    //!< Hidden setting
    } Flags;

    //! @brief Supported types of the object.
    //! @details Enumeration values are compatible to Qt's QVariant / QMetaType
    typedef enum
    {
        Void = 43,      //!< empty type, size = 0
        Bool = 1,       //!< boolean value, size = 1 byte, stored value can be 0 or 1
        Int = 2,        //!< 32-bit signed integer value
        UInt = 3,       //!< 32-bit unsigned integer value
        LongLong = 4,   //!< 64-bit signed integer value
        ULongLong = 5,  //!< 64-bit unsigned integer value
        Double = 6,     //!< 64-bit double precision floating-point value
        Long = 32,      //!< 32-bit signed integer value
        Short = 33,     //!< 16-bit signed integer value
        Char = 34,      //!< 8-bit signed integer value or character
        ULong = 35,     //!< 32-bit unsigned integer value
        UShort = 36,    //!< 16-bit unsigned integer value
        UChar = 37,     //!< 8-bit unsigned integer value
        Float = 38,     //!< 32-bit single precision floating-point value
        SChar = 40,     //!< 8-bit signed integer value

        String = 10,    //!< string encoded in UTF-8 format
        Common = 12,    //!< common value of arbitrary type, can be fixed-size or variable-size, represented as byte array
        Variant = 41,   //!< any-type value (not supported yet)
    } Type;

    typedef enum
    {
        MV_Value    = 0,
        MV_Min      = 1,
        MV_Max      = 2,
        MV_Def      = 3,
        MV_Step     = 4,
        MV_Mime     = 5,
        MV_Hint     = 6,
        MV_Unit     = 7,
        MV_Options  = 8,
        MV_RMIP     = 9,
        MV_Enum     = 10
    } MetaValue;

    ObjectBase() :
        id(m_description.id), RMIP(m_RMIP), timestamp(m_timestamp),
        m_changed(false), m_needTimestamp(false), m_RMIP(0), m_autoPeriodMs(0), m_timestamp(0)
    {}

//    //! @brief Construct Object with given description.
//    //! @param desc Description received from component
//    ObjectBase(const ObjectDescription &desc) :
//        id(m_description.id), RMIP(m_RMIP), timestamp(m_timestamp),
//        m_description(desc),
//        m_changed(false), m_needTimestamp(false), m_RMIP(0), m_autoPeriodMs(0), m_timestamp(0)
//    {
//    }

    ObjectBase(const ObjectBase &) = delete; //important! don't remove this line!!
    void operator =(const ObjectBase &) = delete; //important! don't remove this line!!
    virtual ~ObjectBase() {}

    bool read(QByteArray &ba) {return readMeta(ba, MV_Value);}
    bool write(const QByteArray &ba)
    {
        bool result = writeMeta(ba, MV_Value);
        return result;
    }
    virtual bool readMeta(QByteArray &ba, MetaValue meta) = 0;
    virtual bool writeMeta(const QByteArray &ba, MetaValue meta) = 0;

    bool testExtFlag(MetaValue meta) const
    {
        if (meta == MV_Value)
            return true;
        return m_description.extFlags & (1 << (meta - 1));
    }

    //! @brief Get description of the object.
    //! @return The description.
    const ObjectDescription &description() const {return m_description;}

    const unsigned char &id;

    //! @brief Name of the object.
    //! @return String with the name.
    QString name() const {return m_description.name;}

    Type type() const {return static_cast<Type>(m_description.type);}
    QString typeName() const {return QVariant::typeToName(m_description.type);}

    //! Get access flags.
    Flags flags() const {return static_cast<Flags>(m_description.flags);}

    //! Get default sampling interval.
    const int &RMIP;

    //! Get timestamp of the last received value.
    const uint32_t &timestamp;

protected:
    ObjectDescription m_description;
    bool m_changed;
    bool m_needTimestamp;
    int m_RMIP;
    int m_autoPeriodMs;
    uint32_t m_timestamp;

    QString m_mimeType, m_hint, m_unit, m_options;
    QStringList m_enum;

    QByteArray read()
    {
        QByteArray ba;
        readMeta(ba, MV_Value);
        return ba;
    }

    virtual void setEvent() {}//qDebug() << "[TEST] ObjectBase was set:" << name();}
    virtual void requestEvent() {}//qDebug() << "[TEST] ObjectBase requested:" << name();}
    virtual void receiveEvent() {}//qDebug() << "[TEST] ObjectBase received:" << name();}
    virtual void changeEvent() {}//qDebug() << "[TEST] ObjectBase changed:" << name();}

    void setExtFlag(MetaValue meta)
    {
        m_description.extFlags |= (1 << (meta - 1));
    }

    void setId(unsigned char id, unsigned char group = 0)
    {
        m_description.id = id;
        m_description.group = group;
    }

    template<typename T> static int sizeOfVar(T &) {return sizeof(T);}
    static int sizeOfVar(QString &) {return 0;}
    static int sizeOfVar(QByteArray &) {return 0;}
    static int sizeOfVar(QVariant &) {return 0;}

    template<typename T> static Type typeOfVar(T &) {return Common;}
    static Type typeOfVar(bool &) {return Bool;}
//    static Type typeOfVar(int &) {return Int;}
//    static Type typeOfVar(unsigned int &) {return UInt;}
    static Type typeOfVar(int64_t &) {return LongLong;}
    static Type typeOfVar(uint64_t &) {return ULongLong;}
    static Type typeOfVar(double &) {return Double;}
    static Type typeOfVar(int32_t &) {return Long;}
    static Type typeOfVar(int16_t &) {return Short;}
    static Type typeOfVar(char &) {return Char;}
    static Type typeOfVar(uint32_t &) {return ULong;}
    static Type typeOfVar(uint16_t &) {return UShort;}
    static Type typeOfVar(uint8_t &) {return UChar;}
    static Type typeOfVar(float &) {return Float;}
    static Type typeOfVar(int8_t &) {return SChar;}
    static Type typeOfVar(QString &) {return String;}
    static Type typeOfVar(QByteArray &) {return Common;}
    static Type typeOfVar(QVariant &) {return Variant;}

    friend class ComponentBase;
    friend class ComponentProxyONB;

    //! @brief Create JSON object with info about the ONB Object.
    //! @return JSON object with key-value pairs containing Object info.
    QJsonObject createJsonInfo() const
    {
        QJsonObject obj;
        obj["name"] = name();
        obj["flag"] = flags() == Input ? "Input" : "Output"; //! @todo: there is XEPb, govnoflags in ObjectBase JSON description!!
        obj["type"] = QString::number(m_description.type);
        return obj;
    }

    //! @brief Test an access flag.
    //! @{
    inline bool isVolatile() const {return m_description.flags & Volatile;}
    inline bool isReadable() const {return m_description.flags & Read;}
    inline bool isWritable() const {return m_description.flags & Write;}
    inline bool isStorable() const {return m_description.flags & Save;}
//    inline bool isDual() const {return m_description.flags & Dual;}
    inline bool isHidden() const {return m_description.flags & Hidden;}
    inline bool isFunction() const {return m_description.flags & Function;}
    inline bool isArray() const {return m_description.flags & Array;}
    //! @}
};

template<typename T>
class xoObjectBase : public virtual ObjectBase
{
public:
    xoObjectBase() : m_value(0), m_ptr(&m_value)
    {
        m_description.size = sizeOfVar(m_value);
        m_description.type = typeOfVar(m_value);
    }
    xoObjectBase(const T &copyval) : m_value(copyval), m_ptr(&m_value)
    {
        m_description.size = sizeOfVar(m_value);
        m_description.type = typeOfVar(m_value);
    }
    T &value() {return *m_ptr;}
    const T &value() const {return *m_ptr;}
    operator T&() {return *m_ptr;}
    operator const T&() const {return *m_ptr;}

    T *operator &() {return m_ptr;}
    const T *operator&() const {return m_ptr;}

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
          case MV_RMIP: m_RMIP = *reinterpret_cast<const int32_t*>(ba.constData()); break;
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

private:
    void doRead(QByteArray &ba, const T *ptr)
    {
        ba.append(reinterpret_cast<const char*>(ptr), m_description.size);
    }

    bool doWrite(const QByteArray &ba, T *ptr)
    {
        if (ba.size() != m_description.size)
            return false;
        bool changed = memcmp(ptr, ba.data(), ba.size());
        memcpy(ptr, ba.data(), static_cast<size_t>(ba.size()));
        return changed;
    }
};

template<>
xoObjectBase<QString>::xoObjectBase() :
    m_ptr(&m_value)
{
    m_description.type = String;
}

template<>
void xoObjectBase<QString>::doRead(QByteArray &ba, const QString *ptr)
{
//    qDebug() << "read from string spec";
    ba.append(ptr->toUtf8());
}

template<>
bool xoObjectBase<QString>::doWrite(const QByteArray &ba, QString *ptr)
{
//    qDebug() << "write from string spec";
    QString newstr = QString::fromUtf8(ba.data(), ba.size());
    bool changed = (*ptr != newstr);
    *ptr = newstr;
    return changed;
}

template<>
void xoObjectBase<QByteArray>::doRead(QByteArray &ba, const QByteArray *ptr)
{
//    qDebug() << "read from bytearray spec";
    ba.append(*ptr);
}

template<>
bool xoObjectBase<QByteArray>::doWrite(const QByteArray &ba, QByteArray *ptr)
{
//    qDebug() << "write from bytearray spec";
    bool changed = (*ptr != ba);
    *ptr = ba;
    return changed;
}

template<>
void xoObjectBase<QVariant>::doRead(QByteArray &ba, const QVariant *ptr)
{
//    qDebug() << "read from variant spec";
    ba.append(ptr->toString().toUtf8());
}

template<>
bool xoObjectBase<QVariant>::doWrite(const QByteArray &ba, QVariant *ptr)
{
//    qDebug() << "write from variant spec";
    QVariant newvar = QString::fromUtf8(ba.data(), ba.size());
    bool changed = (*ptr != newvar);
    *ptr = newvar;
    return changed;
}

template <typename T>
class xoObject : public xoObjectBase<T>
{
public:
    xoObject() : xoObjectBase<T>() {}
    xoObject(const xoObject<T> &copy)
    {
        *this->m_ptr = *copy.m_ptr;
    }
    xoObject(const T &value) : xoObjectBase<T>(value) {}

    void operator =(const T &v) {xoObjectBase<T>::operator=(v);}
//    {
//        *m_ptr = v;
//        setEvent();
//    }

    //! @brief Set the minimum value of the object.
    //! @attention This meta-value is only for information and does not apply to the variable.
    //! @param value The minimum value to be set.
    //! @return reference to the object.
    xoObject<T> &min(const T &value)
    {
        xoObjectBase<T>::m_min = value;
        setExtFlag(ObjectBase::MV_Min);
        return *this;
    }

    //! @brief Set the maximum value of the object.
    //! @attention This meta-value is only for information and does not apply to the variable.
    //! @param value The maximum value to be set.
    //! @return reference to the object.
    xoObject<T> &max(const T &value)
    {
        xoObjectBase<T>::m_max = value;
        setExtFlag(ObjectBase::MV_Max);
        return *this;
    }

    //! @brief Set the default value of the object.
    //! @attention This meta-value is only for information and does not apply to the variable.
    //! @param value The default value to be set.
    //! @return reference to the object.
    xoObject<T> &def(const T &value)
    {
        xoObjectBase<T>::m_def = value;
        xoObjectBase<T>::m_value = value;
        setExtFlag(ObjectBase::MV_Def);
        return *this;
    }

    //! @brief Set the step value of the object.
    //! @details Step is used commonly for changing the value in discrete steps.
    //! @attention This meta-value is only for information and does not apply to the variable.
    //! @param value The step value to be set.
    //! @return reference to the object.
    xoObject<T> &step(const T &value)
    {
        xoObjectBase<T>::m_step = value;
        setExtFlag(ObjectBase::MV_Step);
        return *this;
    }

    //! @brief Set the minimum, maximum and default value of the object.
    //! @details The method is implemented as consequent call to min(), max() and def()
    //! and can be used to simplify the code.
    //! @param minValue The minimum value to be set.
    //! @param maxValue The maximum value to be set.
    //! @param defValue The default value to be set.
    //! @return reference to the object.
    //! @see min(), max(), def().
    xoObject<T> &MMD(const T &minValue, const T &maxValue, const T &defValue)
    {
        return min(minValue).max(maxValue).def(defValue);
    }

    //! @brief Set the mime-type of the object.
    //! @details Mime-type is arbitrary text used for specifying content of the object.
    //! \n For example:
    //! @code
    //!  QByteArray frameBuffer;
    //!  createOutput("frame", frameBuffer).mime("image/png");
    //! @endcode
    //! @param value The string containing mime-type to be set.
    //! @return reference to the object.
    //! @see ComponentBase::createOutput()
    xoObject<T> &mime(const QString &value)
    {
        xoObjectBase<T>::m_mimeType = value;
        setExtFlag(ObjectBase::MV_Mime);
        return *this;
    }

    //! @brief Set the human-readable description of the object.
    //! @details The description helps users to understand purpose of the object.\n
    //! For example, if you'd develop some kind of PID controller
    //! you probably should write like this:
    //! @code
    //!  createSetting("Kp", Kp).hint("The coefficient for the proportional term");
    //! @endcode
    //! @param value The string containing description to be set.
    //! @return reference to the object.
    //! @see ComponentBase::createSetting()
    xoObject<T> &hint(const QString &value)
    {
        xoObjectBase<T>::m_hint = value;
        setExtFlag(ObjectBase::MV_Hint);
        return *this;
    }

    //! @brief Set the unit of the object.
    //! @details Unit is arbitrary text used for specifying measure of the value of the object.
    //! @code
    //!  createInput("frequency", freq).unit("Hz");
    //! @endcode
    //! @param value The string containing unit to be set.
    //! @return reference to the object.
    //! @see ComponentBase::createInput()
    xoObject<T> &unit(const QString &value)
    {
        xoObjectBase<T>::m_unit = value;
        setExtFlag(ObjectBase::MV_Unit);
        return *this;
    }

    //! @brief Set the optional text associated to the object.
    //! @details Can contain arbitrary text, but some special words are used for UI enhancement.
    //! @param value The string of optional text to be set.
    //! @return reference to the object.
    xoObject<T> &opt(const QString &value)
    {
        xoObjectBase<T>::m_options = value;
        setExtFlag(ObjectBase::MV_Options);
        return *this;
    }

    //! @brief Set the default sampling interval of the object.
    //! @details It is applicable to inputs or outputs and denotes the recommended time interval
    //! to request or send the value of the object.
    //! If the sampling interval is set, the object will include timestamp.
    //! and can be used in real-time dataflow.
    //! @param value The sampling interval. If the special value of zero is set, the object will be sent as soon as it is touched.
    //! @return reference to the object.
    //! @see ComponentBase::touchOutput().
    xoObject<T> &sampling(std::chrono::milliseconds value)
    {
        ObjectBase::m_RMIP = value.count();
        ObjectBase::m_needTimestamp = ObjectBase::m_RMIP > 0; // it's evident
        setExtFlag(ObjectBase::MV_RMIP);
        return *this;
    }

    //! @brief Specify the list of the enumeration object values.
    //! @details The object can be bound to integer variable, the string or the native enumeration.
    //! If the object bound to integer variable, it defines the minimum and maximum values
    //! as 0 and (count-1) respectively.
    //! If the object bound to the string, the given names are the possible values of the string.
    //! @code
    //!  createSetting("Shape", shape).enumeration("Square", "Circle", "Triangle");
    //! @endcode
    //! @param s The first string to be set.
    //! @param a The next strings, you can provide arbitrary argument count.
    //! @return reference to the object.
    //! @see ComponentBase::createSetting()
    template<typename... Args>
    xoObject<T> &enumeration(QString s, Args... a)
    {
        xoObjectBase<T>::m_enum << s;
        return enumeration(a...);
    }

    //! @brief Specify the names of the enumeration object with given string list.
    //! @details This is overloaded function.
    //! @param list The string list to be set.
    //! @return reference to the object.
    //! @see ONBObject::enumeration(QString s, Args... a)
    template<typename... Args>
    xoObject<T> &enumeration(QStringList list)
    {
        for (auto item: list)
            xoObjectBase<T>::m_enum << item;
        return enumeration();
    }
};

typedef xoObject<bool>          xoBool;
typedef xoObject<char>          xoChar;
typedef xoObject<int>           xoInt;
typedef xoObject<unsigned int>  xoUInt;
typedef xoObject<int8_t>        xoInt8;
typedef xoObject<uint8_t>       xoUInt8;
typedef xoObject<int16_t>       xoInt16;
typedef xoObject<uint16_t>      xoUInt16;
typedef xoObject<int32_t>       xoInt32;
typedef xoObject<uint32_t>      xoUInt32;
typedef xoObject<int64_t>       xoInt64;
typedef xoObject<uint64_t>      xoUInt64;
typedef xoObject<float>         xoFloat;
typedef xoObject<double>        xoDouble;
typedef xoObject<QString>       xoString;
typedef xoObject<QByteArray>    xoByteArray;
typedef xoObject<QVariant>      xoVariant;


//template<typename T>
//class ONBObject;

////! @brief ONB Object base interface.
////! @details This is fundamental entity in ONB protocol.
////! Data exchange is carried out by means of Objects transfer.
////! Do not create objects of this class. Use methods of ComponentBase instead.
////! @see ONBObject, ComponentBase
//class XOTOOLSSHARED_EXPORT ObjectInfo
//{
//public:
//    //! Access flags of the Object.
//    typedef enum
//    {
//        Volatile= 0x01, //!< Volatile Object is input or output, changes in real-time.
//        Read    = 0x02, //!< Object can be read
//        Write   = 0x04, //!< Object can be written
//        Save    = 0x08, //!< Object value is stored in NVM
//        Hidden  = 0x10, //!< Object is hidden for users, may be considered for internal use
//        Dual    = 0x20, //!< Object is read from one location, written to another
//        Function= 0x40, //!< Object is the function (now supported in devices only)
//        Array   = 0x80, //!< Object is array (now supported in devices only)

//        ReadOnly    = Read,         //!< Alias for read-only access
//        WriteOnly   = Write,        //!< Alias for write-only access
//        ReadWrite   = Read | Write, //!< Alias for read-write access

//        Constant    = Read,             //!< Constant object is not volatile and can only be read
//        Input       = Write | Volatile, //!< Input is volatile object that can oly be written
//        Output      = Read | Volatile,  //!< Output is volatile
//        Variable    = Read | Write,     //!< Common object with read and write access
//        Setting     = Variable | Save,  //!< Object with read-write access, its value stored in NVM

//        Exchange    = Dual | ReadWrite | Volatile, //!< Special bi-directional Object (in-out)
//        SecretVar   = ReadWrite | Hidden, //!< Common hidden object
//        SecretSetting = Setting | Hidden //!< Hidden setting
//    } Flags;

//    //! @brief Supported types of the object.
//    //! @details Enumeration values are compatible to Qt's QVariant / QMetaType
//    typedef enum
//    {
//        Void = 43,      //!< empty type, size = 0
//        Bool = 1,       //!< boolean value, size = 1 byte, stored value can be 0 or 1
//        Int = 2,        //!< 32-bit signed integer value
//        UInt = 3,       //!< 32-bit unsigned integer value
//        LongLong = 4,   //!< 64-bit signed integer value
//        ULongLong = 5,  //!< 64-bit unsigned integer value
//        Double = 6,     //!< 64-bit double precision floating-point value
//        Long = 32,      //!< 32-bit signed integer value
//        Short = 33,     //!< 16-bit signed integer value
//        Char = 34,      //!< 8-bit signed integer value or character
//        ULong = 35,     //!< 32-bit unsigned integer value
//        UShort = 36,    //!< 16-bit unsigned integer value
//        UChar = 37,     //!< 8-bit unsigned integer value
//        Float = 38,     //!< 32-bit single precision floating-point value
//        SChar = 40,     //!< 8-bit signed integer value

//        String = 10,    //!< string encoded in UTF-8 format
//        Common = 12,    //!< common value of arbitrary type, can be fixed-size or variable-size, represented as byte array
//        Variant = 41,   //!< any-type value (not supported yet)
//    } Type;

//protected:
////! @privatesection
//    typedef enum
//    {
//        Minimum = 0x0001,
//        Maximum = 0x0002,
//        Default = 0x0004,
//        MMD = Minimum | Maximum | Default,
//        Step    = 0x0008,
//        MimeType= 0x0010,
//        Hint    = 0x0020,
//        Unit    = 0x0040,
//        Options = 0x0080,
//        ExtInfo = 0x0100,
//        Enum    = 0x0200,
//    } ExtFlags;

//    typedef enum
//    {
//        MV_Min      = 1,
//        MV_Max      = 2,
//        MV_Def      = 3,
//        MV_Step     = 4,
//        MV_Mime     = 5,
//        MV_Hint     = 6,
//        MV_Unit     = 7,
//        MV_Options  = 8,
//        MV_ExtInfo  = 9,
//        MV_Enum     = 10
//    } MetaValue;

//    typedef bool Bool_t;
//    typedef int Int_t;
//    typedef unsigned int UInt_t;
//    typedef long long LongLong_t;
//    typedef unsigned long long  ULongLong_t;
//    typedef double Double_t;
//    typedef long Long_t;
//    typedef short Short_t;
//    typedef char Char_t;
//    typedef unsigned long ULong_t;
//    typedef unsigned short UShort_t;
//    typedef unsigned char UChar_t;
//    typedef float Float_t;
//    typedef signed char SChar_t;
//    typedef QString String_t;

//    template<typename T> static Type typeOfVar(T &var) {(void)var; return Common;}
//#define DeclareTypeOfVar(Tp) static Type typeOfVar(Tp##_t &) {return Tp;}
//    //DeclareTypeOfVar(Void)
//    DeclareTypeOfVar(Bool)
//    DeclareTypeOfVar(Int)
//    DeclareTypeOfVar(UInt)
//    DeclareTypeOfVar(LongLong)
//    DeclareTypeOfVar(ULongLong)
//    DeclareTypeOfVar(Double)
//    DeclareTypeOfVar(Long)
//    DeclareTypeOfVar(Short)
//    DeclareTypeOfVar(Char)
//    DeclareTypeOfVar(ULong)
//    DeclareTypeOfVar(UShort)
//    DeclareTypeOfVar(UChar)
//    DeclareTypeOfVar(Float)
//    DeclareTypeOfVar(SChar)
//    DeclareTypeOfVar(String)
////    static Type typeOfVar(String_t &) {return String;}
////    template<> ObjectInfo::Type typeOfVar<_String>(_String &var) {(void)var; return ObjectInfo::String;}
////    DeclareTypeOfVar(QVector3D)
////    DeclareTypeOfVar(QQuaternion)

//    class ExtendedInfo
//    {
//    public:
//        int RMIP;           // РМИП
//        int autoPeriodMs;   // period of automatic output generation
////        bool sendOnlyIfChanged;
//        bool needTimestamp; // The need of timestamp transmission

//        ExtendedInfo() : RMIP(10), autoPeriodMs(0), needTimestamp(false) {}
//    };

//    const void *mReadPtr = nullptr;
//    void *mWritePtr = nullptr;
//    ObjectDescription mDesc;
//    ExtendedInfo m_extInfo;
//    bool m_changed = false;
//    uint32_t m_timestamp = 0;

//    int sizeofType(Type type) const;

//    bool doRead(QByteArray &ba, const void *ptr) const;
//    bool doWrite(const QByteArray &ba, void *ptr);

//    virtual void requestEvent() const {}
//    virtual void receiveEvent() {}
//    virtual void valueChangeEvent() {}

//    friend class ComponentBase;
//    friend class ComponentProxyONB;

//    //! @brief Variable binding.
//    //! @details You don't need call this method directly.
//    //! Use more convenient methods of ONBObject class.
//    //! The main principle of ONB is implicit Object exchange.
//    //! So you should bind some variable to ONB Object and then use it as usual.
//    //! ONB takes care about object exchange between component and the core,
//    //! see ComponentBase class for details.
//    //! @param name Object name. Must be unique in the component.
//    //! @param var Reference to variable for binding.
//    //! @attention Variable must not be deleted or moved after it is binded!
//    //! There is a way to rebind the object using ComponentBase::rebind();
//    //! @param flags Access flags.
//    //! @return The created object.
//    //! @see ONBObject::createInput(), ONBObject::createOutput(), ONBObject::createSetting()
//    template<typename T>
//    static ONBObject<T> &create(QString name, T &var, Flags flags=ReadWrite, bool in_is_ba =false)
//    {
//        ONBObject<T> *obj = new ONBObject<T>;
//        unsigned short sz = in_is_ba ? 0 : (unsigned short)sizeof(T);
//        Type t = typeOfVar(var);
//        if (flags & Read)
//        {
//            obj->mReadPtr = &var;
//            obj->mDesc.readSize = sz;
//            obj->mDesc.rType = t;
//        }
//        if (flags & Write)
//        {
//            obj->mWritePtr = &var;
//            obj->mDesc.writeSize = sz;
//            obj->mDesc.wType = t;
//        }
//        obj->mDesc.flags = flags;
//        obj->mDesc.name = name;
//        return *obj;
//    }

//#ifdef _MSC_VER
//    //! @brief Byte array binding.
//    //! @see Variable binding.
//    template<> static ONBObject<QByteArray> &create(QString name, QByteArray &var, Flags flags,bool in_is_ba);
//#endif

//    //! @brief Array binding.
//    //! Not supported yet.
//    //! @see Variable binding.
//    template<typename T, int N>
//    static ONBObject<T> &create(QString name, T (&var)[N], Flags flags=ReadWrite, bool in_is_ba =false)
//    {
//        ONBObject<T> *obj = new ONBObject<T>;
//        unsigned short sz = in_is_ba ? 0 : static_cast<unsigned short>(sizeof(T)) * N;
//        Type t = typeOfVar(var[0]);
//        if (flags & Read)
//        {
//            obj->mReadPtr = &var;
//            obj->mDesc.readSize = sz;
//            obj->mDesc.rType = t;
//        }
//        if (flags & Write)
//        {
//            obj->mWritePtr = &var;
//            obj->mDesc.writeSize = sz;
//            obj->mDesc.wType = t;
//        }
//        obj->mDesc.flags = flags | Array;
//        obj->mDesc.name = name;
//        return *obj;
//    }

//    template<typename T>
//    bool rebind(T &var)
//    {
//        bool descChanged = false;
//        unsigned short sz = (unsigned short)sizeof(T);
//        Type t = typeOfVar(var);
//        if (mDesc.flags & Read)
//        {
//            if (t != mDesc.rType || sz != mDesc.readSize)
//                descChanged = true;
//            mDesc.rType = t;
//            mDesc.readSize = sz;
//            mReadPtr = &var;
//        }
//        if (mDesc.flags & Write)
//        {
//            if (t != mDesc.wType || sz != mDesc.writeSize)
//                descChanged = true;
//            mDesc.wType = t;
//            mDesc.writeSize = sz;
//            mWritePtr = &var;
//        }
//        return descChanged;
//    }

//    void setId(unsigned char oid, unsigned char group = 0) {mDesc.id = oid; mDesc.group = group;}

//    //! Read the bound variable.
//    //! @return Byte array containing serialized value of the object.
//    QByteArray read() const;

//    //! Read the bound variable.
//    //! @param ba Reference to byte array to append serialized value of the object.
//    //! @return true if succeeded.
//    bool read(QByteArray &ba) const;

//    //! Read the meta-value of the object.
//    //! @param ba Reference to byte array to append serialized meta-value of the object.
//    //! @param id meta-value index.
//    virtual bool readMeta(QByteArray &ba, MetaValue id) const {(void)id; return read(ba);}

//    bool write(const QByteArray &ba);

//    virtual bool writeMeta(const QByteArray &ba, MetaValue id) {(void)ba; (void)id; return false;}

//public:
//    //! @brief Common constructor.
//    //! Initializes unbound object.
//    ObjectInfo();

//    //! @brief Construct Object with given description.
//    //! @param desc Description received from component
//    ObjectInfo(const ObjectDescription &desc);

//    //! no copy allowed!!
//    ObjectInfo(const ObjectInfo &other) = delete;

////    virtual ~ObjectInfo() {}

//    //! @brief Get description of the object.
//    //! @return The description.
//    const ObjectDescription &description() const {return mDesc;}

//    //! @brief Get name of the object.
//    //! @return String containing the name.
//    QString name() const {return mDesc.name;}

//    //! Get access flags.
//    Flags flags() const {return static_cast<Flags>(mDesc.flags);}

//    //! Get default sampling interval.
//    int RMIP() const {return m_extInfo.RMIP;}

//    //! Get timestamp of the last received value.
//    uint32_t timestamp() const {return m_timestamp;}

//    //! @brief Test an access flag.
//    //! @{
//    inline bool isVolatile() const {return mDesc.flags & Volatile;}
//    inline bool isReadable() const {return mDesc.flags & Read;}
//    inline bool isWritable() const {return mDesc.flags & Write;}
//    inline bool isStorable() const {return mDesc.flags & Save;}
//    inline bool isDual() const {return mDesc.flags & Dual;}
//    inline bool isHidden() const {return mDesc.flags & Hidden;}
//    inline bool isFunction() const {return mDesc.flags & Function;}
//    inline bool isArray() const {return mDesc.flags & Array;}
//    //! @}

//    //! @brief Test a meta-value flag.
//    //! @{
//    bool hasMinimum() const {return mDesc.extFlags & Minimum;}
//    bool hasMaximum() const {return mDesc.extFlags & Maximum;}
//    bool hasDefault() const {return mDesc.extFlags & Default;}
//    bool hasStep() const {return mDesc.extFlags & Step;}
//    bool hasMimeType() const {return mDesc.extFlags & MimeType;}
//    bool hasHint() const {return mDesc.extFlags & Hint;}
//    bool hasUnit() const {return mDesc.extFlags & Unit;}
//    bool hasOptions() const {return mDesc.extFlags & Options;}
//    bool hasExtInfo() const {return mDesc.extFlags & ExtInfo;}
//    bool isEnum() const {return mDesc.extFlags & Enum;}
//    //! @}

//    //! @brief Create JSON object with info about the ONB Object.
//    //! @return JSON object with key-value pairs containing Object info.
//    QJsonObject createJsonInfo() const;
//};

#endif // OBJECTINFO_H
