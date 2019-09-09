#ifndef XOTOOLS_GLOBAL_H
#define XOTOOLS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(XOTOOLS_LIBRARY)
#  define XOTOOLSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define XOTOOLSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // XOTOOLS_GLOBAL_H
