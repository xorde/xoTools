#ifndef XOTYPES_H
#define XOTYPES_H

#include "xoObject.h"

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

#endif // XOTYPES_H
