#ifndef OBJECTINFO_H
#define OBJECTINFO_H

#include "xotools_global.h"
#include "objectdescription.h"
#include <QJsonObject>
#include <QDebug>

class ComponentBase;

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
        UnknownType = 0,    //!< not initialized type

        Void = 43,      //!< empty type, size = 0
        Bool = 1,       //!< boolean value, size = 1 byte, stored value can be 0 or 1
        Integer = 2,    //!< signed integer value (size depends on the underlying type)
        UInteger = 3,   //!< unsigned integer value (size depends on the underlying type)
        Double = 6,     //!< 64-bit double precision floating-point value
        Char = 34,      //!< Character (now it is 8-bit signed integer value and somehow supported)
        Float = 38,     //!< 32-bit single precision floating-point value

        String = 10,    //!< string encoded in UTF-8 format
        Common = 12,    //!< common value of arbitrary type, can be fixed-size or variable-size, represented as byte array
        Variant = 41,   //!< any-type value (not tested yet)
        Image = 70      //!< picture (not tested yet)
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

    ObjectBase();

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
    virtual ~ObjectBase();

    bool read(QByteArray &ba);
    bool write(const QByteArray &ba);
    virtual bool readMeta(QByteArray &ba, MetaValue meta) = 0;
    virtual bool writeMeta(const QByteArray &ba, MetaValue meta) = 0;

    bool testExtFlag(MetaValue meta) const;

    //! @brief Get description of the object.
    //! @return The description.
    const ObjectDescription &description() const {return m_description;}

    const unsigned char &id;

    //! @brief Name of the object.
    //! @return String with the name.
    QString name() const {return m_description.name;}

    Type type() const {return static_cast<Type>(m_description.type);}
    QString typeName() const;

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

    QByteArray read();

    void setEvent();
    virtual void requestEvent() {}//qDebug() << "[TEST] ObjectBase requested:" << name();}
    virtual void receiveEvent() {}//qDebug() << "[TEST] ObjectBase received:" << name();}
    virtual void changeEvent() {}//qDebug() << "[TEST] ObjectBase changed:" << name();}

    void setExtFlag(MetaValue meta);

    void setId(unsigned char id, unsigned char group = 0);

    friend class ComponentBase;
    friend class ComponentProxyONB;

    //! @brief Create JSON object with info about the ONB Object.
    //! @return JSON object with key-value pairs containing Object info.
    QJsonObject createJsonInfo() const;

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

private:
    ComponentBase *m_component; // DIRTY HACK
};

#endif // OBJECTINFO_H
