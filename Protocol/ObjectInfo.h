#ifndef OBJECTINFO_H
#define OBJECTINFO_H

#include "xotools_global.h"
#include "objectdescription.h"
#include <QJsonObject>

template<typename T>
class ONBObject;

//! @brief ONB Object base interface.
//! @details This is fundamental entity in ONB protocol.
//! Data exchange is carried out by means of Objects transfer.
//! Do not create objects of this class. Use methods of ComponentBase instead.
//! @see ONBObject, ComponentBase
class XOTOOLSSHARED_EXPORT ObjectInfo
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
        Dual    = 0x20, //!< Object is read from one location, written to another
        Function= 0x40, //!< Object is the function (now supported in devices only)
        Array   = 0x80, //!< Object is array (now supported in devices only)

        ReadOnly    = Read,         //!< Alias for read-only access
        WriteOnly   = Write,        //!< Alias for write-only access
        ReadWrite   = Read | Write, //!< Alias for read-write access

        Constant    = Read,             //!< Constant object is not volatile and can only be read
        Input       = Write | Volatile, //!< Input is volatile object that can oly be written
        Output      = Read | Volatile,  //!< Output is volatile
        Variable    = Read | Write,     //!< Common object with read and write access
        Setting     = Variable | Save,  //!< Object with read-write access, its value stored in NVM

        Exchange    = Dual | ReadWrite | Volatile, //!< Special bi-directional Object (in-out)
        SecretVar   = ReadWrite | Hidden, //!< Common hidden object
        SecretSetting = Setting | Hidden //!< Hidden setting
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

protected:
//! @privatesection
    typedef enum
    {
        Minimum = 0x0001,
        Maximum = 0x0002,
        Default = 0x0004,
        MMD = Minimum | Maximum | Default,
        Step    = 0x0008,
        MimeType= 0x0010,
        Hint    = 0x0020,
        Unit    = 0x0040,
        Options = 0x0080,
        ExtInfo = 0x0100,
        Enum    = 0x0200,
    } ExtFlags;

    typedef enum
    {
        MV_Min      = 1,
        MV_Max      = 2,
        MV_Def      = 3,
        MV_Step     = 4,
        MV_Mime     = 5,
        MV_Hint     = 6,
        MV_Unit     = 7,
        MV_Options  = 8,
        MV_ExtInfo  = 9,
        MV_Enum     = 10
    } MetaValue;

    typedef bool Bool_t;
    typedef int Int_t;
    typedef unsigned int UInt_t;
    typedef long long LongLong_t;
    typedef unsigned long long  ULongLong_t;
    typedef double Double_t;
    typedef long Long_t;
    typedef short Short_t;
    typedef char Char_t;
    typedef unsigned long ULong_t;
    typedef unsigned short UShort_t;
    typedef unsigned char UChar_t;
    typedef float Float_t;
    typedef signed char SChar_t;
    typedef QString String_t;

    template<typename T> static Type typeOfVar(T &var) {(void)var; return Common;}
#define DeclareTypeOfVar(Tp) static Type typeOfVar(Tp##_t &) {return Tp;}
    //DeclareTypeOfVar(Void)
    DeclareTypeOfVar(Bool)
    DeclareTypeOfVar(Int)
    DeclareTypeOfVar(UInt)
    DeclareTypeOfVar(LongLong)
    DeclareTypeOfVar(ULongLong)
    DeclareTypeOfVar(Double)
    DeclareTypeOfVar(Long)
    DeclareTypeOfVar(Short)
    DeclareTypeOfVar(Char)
    DeclareTypeOfVar(ULong)
    DeclareTypeOfVar(UShort)
    DeclareTypeOfVar(UChar)
    DeclareTypeOfVar(Float)
    DeclareTypeOfVar(SChar)
    DeclareTypeOfVar(String)
//    static Type typeOfVar(String_t &) {return String;}
//    template<> ObjectInfo::Type typeOfVar<_String>(_String &var) {(void)var; return ObjectInfo::String;}
//    DeclareTypeOfVar(QVector3D)
//    DeclareTypeOfVar(QQuaternion)

    class ExtendedInfo
    {
    public:
        int RMIP;           // РМИП
        int autoPeriodMs;   // period of automatic output generation
//        bool sendOnlyIfChanged;
        bool needTimestamp; // The need of timestamp transmission

        ExtendedInfo() : RMIP(10), autoPeriodMs(0), needTimestamp(false) {}
    };

    const void *mReadPtr = nullptr;
    void *mWritePtr = nullptr;
    ObjectDescription mDesc;
    ExtendedInfo m_extInfo;
    bool m_changed = false;
    uint32_t m_timestamp = 0;

    int sizeofType(Type type) const;

    bool doRead(QByteArray &ba, const void *ptr) const;
    bool doWrite(const QByteArray &ba, void *ptr);

    virtual void requestEvent() const {}
    virtual void receiveEvent() {}
    virtual void valueChangeEvent() {}

    friend class ComponentBase;
    friend class ComponentProxyONB;

    //! @brief Variable binding.
    //! @details You don't need call this method directly.
    //! Use more convenient methods of ONBObject class.
    //! The main principle of ONB is implicit Object exchange.
    //! So you should bind some variable to ONB Object and then use it as usual.
    //! ONB takes care about object exchange between component and the core,
    //! see ComponentBase class for details.
    //! @param name Object name. Must be unique in the component.
    //! @param var Reference to variable for binding.
    //! @attention Variable must not be deleted or moved after it is binded!
    //! There is a way to rebind the object using ComponentBase::rebind();
    //! @param flags Access flags.
    //! @return The created object.
    //! @see ONBObject::createInput(), ONBObject::createOutput(), ONBObject::createSetting()
    template<typename T>
    static ONBObject<T> &create(QString name, T &var, Flags flags=ReadWrite)
    {
        ONBObject<T> *obj = new ONBObject<T>;
        unsigned short sz = (unsigned short)sizeof(T);
        Type t = typeOfVar(var);
        if (flags & Read)
        {
            obj->mReadPtr = &var;
            obj->mDesc.readSize = sz;
            obj->mDesc.rType = t;
        }
        if (flags & Write)
        {
            obj->mWritePtr = &var;
            obj->mDesc.writeSize = sz;
            obj->mDesc.wType = t;
        }
        obj->mDesc.flags = flags;
        obj->mDesc.name = name;
        return *obj;
    }

#ifdef _MSC_VER
    //! @brief Byte array binding.
    //! @see Variable binding.
    template<> static ONBObject<QByteArray> &create(QString name, QByteArray &var, Flags flags);
#endif

    //! @brief Array binding.
    //! Not supported yet.
    //! @see Variable binding.
    template<typename T, int N>
    static ONBObject<T> &create(QString name, T (&var)[N], Flags flags=ReadWrite)
    {
        ONBObject<T> *obj = new ONBObject<T>;
        unsigned short sz = static_cast<unsigned short>(sizeof(T)) * N;
        Type t = typeOfVar(var[0]);
        if (flags & Read)
        {
            obj->mReadPtr = &var;
            obj->mDesc.readSize = sz;
            obj->mDesc.rType = t;
        }
        if (flags & Write)
        {
            obj->mWritePtr = &var;
            obj->mDesc.writeSize = sz;
            obj->mDesc.wType = t;
        }
        obj->mDesc.flags = flags | Array;
        obj->mDesc.name = name;
        return *obj;
    }

    template<typename T>
    bool rebind(T &var)
    {
        bool descChanged = false;
        unsigned short sz = (unsigned short)sizeof(T);
        Type t = typeOfVar(var);
        if (mDesc.flags & Read)
        {
            if (t != mDesc.rType || sz != mDesc.readSize)
                descChanged = true;
            mDesc.rType = t;
            mDesc.readSize = sz;
            mReadPtr = &var;
        }
        if (mDesc.flags & Write)
        {
            if (t != mDesc.wType || sz != mDesc.writeSize)
                descChanged = true;
            mDesc.wType = t;
            mDesc.writeSize = sz;
            mWritePtr = &var;
        }
        return descChanged;
    }

    void setId(unsigned char oid, unsigned char group = 0) {mDesc.id = oid; mDesc.group = group;}

    //! Read the bound variable.
    //! @return Byte array containing serialized value of the object.
    QByteArray read() const;

    //! Read the bound variable.
    //! @param ba Reference to byte array to append serialized value of the object.
    //! @return true if succeeded.
    bool read(QByteArray &ba) const;

    //! Read the meta-value of the object.
    //! @param ba Reference to byte array to append serialized meta-value of the object.
    //! @param id meta-value index.
    virtual bool readMeta(QByteArray &ba, MetaValue id) const {(void)id; return read(ba);}

    bool write(const QByteArray &ba);

    virtual bool writeMeta(const QByteArray &ba, MetaValue id) {(void)ba; (void)id; return false;}

public:
    //! @brief Common constructor.
    //! Initializes unbound object.
    ObjectInfo();

    //! @brief Construct Object with given description.
    //! @param desc Description received from component
    ObjectInfo(const ObjectDescription &desc);

    //! no copy allowed!!
    ObjectInfo(const ObjectInfo &other) = delete;

//    virtual ~ObjectInfo() {}

    //! @brief Get description of the object.
    //! @return The description.
    const ObjectDescription &description() const {return mDesc;}

    //! @brief Get name of the object.
    //! @return String containing the name.
    QString name() const {return mDesc.name;}

    //! Get access flags.
    Flags flags() const {return static_cast<Flags>(mDesc.flags);}

    //! Get default sampling interval.
    int RMIP() const {return m_extInfo.RMIP;}

    //! Get timestamp of the last received value.
    uint32_t timestamp() const {return m_timestamp;}

    //! @brief Test an access flag.
    //! @{
    inline bool isVolatile() const {return mDesc.flags & Volatile;}
    inline bool isReadable() const {return mDesc.flags & Read;}
    inline bool isWritable() const {return mDesc.flags & Write;}
    inline bool isStorable() const {return mDesc.flags & Save;}
    inline bool isDual() const {return mDesc.flags & Dual;}
    inline bool isHidden() const {return mDesc.flags & Hidden;}
    inline bool isFunction() const {return mDesc.flags & Function;}
    inline bool isArray() const {return mDesc.flags & Array;}
    //! @}

    //! @brief Test a meta-value flag.
    //! @{
    bool hasMinimum() const {return mDesc.extFlags & Minimum;}
    bool hasMaximum() const {return mDesc.extFlags & Maximum;}
    bool hasDefault() const {return mDesc.extFlags & Default;}
    bool hasStep() const {return mDesc.extFlags & Step;}
    bool hasMimeType() const {return mDesc.extFlags & MimeType;}
    bool hasHint() const {return mDesc.extFlags & Hint;}
    bool hasUnit() const {return mDesc.extFlags & Unit;}
    bool hasOptions() const {return mDesc.extFlags & Options;}
    bool hasExtInfo() const {return mDesc.extFlags & ExtInfo;}
    bool isEnum() const {return mDesc.extFlags & Enum;}
    //! @}

    //! @brief Create JSON object with info about the ONB Object.
    //! @return JSON object with key-value pairs containing Object info.
    QJsonObject createJsonInfo() const;
};

#endif // OBJECTINFO_H
