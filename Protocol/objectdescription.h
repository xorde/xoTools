#ifndef OBJECTDESCRIPTION_H
#define OBJECTDESCRIPTION_H

#include "xotools_global.h"
#include <QString>
#include <QByteArray>

#pragma pack(push,1)

//! @brief ONB Object description header.
//! @details Old-style description (used for ONB devices)
struct ObjectDescriptionHeaderV1
{
    unsigned char id;           //!< Object ID
    unsigned char flags;        //!< Access flags from Flags enum
    unsigned char rType;        //!< Type of the object to read (Type enum)
    unsigned char wType;        //!< Type of the object to write (Type enum)
    unsigned char readSize;     //!< Size of the object to read in bytes. If type of the object assumes variable size, must be equal to 0.
    unsigned char writeSize;    //!< Size of the object to write in bytes. If type of the object assumes variable size, must be equal to 0.
};

//! @brief ONB Object description header, version 2.
//! @details Contains extended flags for meta-value exchange.
class ObjectDescriptionHeaderV2
{
public:
    unsigned char id;           //!< Object ID
    unsigned char version: 4;   //!< Version of the descriptor (current version = 2)
    unsigned char _verid: 4;    //!< Distinction of descriptor version >= 2 (must be equal to 0xF)
    unsigned char group;        //!< Object ID category
    unsigned char _reserve;     //!< Not used, reserved for future
    unsigned char flags;        //!< Access flags from Flags enum
    unsigned short extFlags;    //!< Advanced flags (min max def) from ExtFlags enum
    unsigned char rType;        //!< Type of the object to read (Type enum)
    unsigned char wType;        //!< Type of the object to write (Type enum)
    unsigned short readSize;    //!< Size of the object to read in bytes. If type of the object assumes variable size, must be equal to 0.
    unsigned short writeSize;   //!< Size of the object to write in bytes. If type of the object assumes variable size, must be equal to 0.

    ObjectDescriptionHeaderV2();
};

//! @brief ONB Object description.
//! @details Contains header and name of the Object.
class XOTOOLSSHARED_EXPORT ObjectDescription : public ObjectDescriptionHeaderV2
{
private:
    inline unsigned char swapRWflags(unsigned char f) {return (f & 0xF9) | ((f & 2) << 1) | ((f & 4) >> 1);}

    //! Unserialize descriptor from byte array.
    //! Old-style descriptor version 1 also supported.
    bool parse(const QByteArray &ba);

public:
    //! Name of the object.
    QString name;

    //! Constructs the description with invalid flags and uninitialized data.
    explicit ObjectDescription();

    //! Constructs the description from serialized data
    ObjectDescription(const QByteArray &ba);

    //! @brief Test Object descriptor for validity.
    //! @return true if the descriptor is valid.
    inline bool isValid() const {return flags;} // valid object must have at least one flag set

    //! Create human-readable representation of flags.
    QString flagString() const;

    //! @brief Serialize the descriptor to byte array.
    //! @details This implementation uses descriptor version 2.
    void read(QByteArray &ba) const;
};
#pragma pack(pop)

#endif // OBJECTDESCRIPTION_H
