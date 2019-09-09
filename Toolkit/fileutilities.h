#ifndef FILEUTILITIES_H
#define FILEUTILITIES_H

#include <QDir>
#include <QFile>
#include <QObject>
#include <QFileDialog>
#include "nlist.h"
#include "xotools_global.h"

class XOTOOLSSHARED_EXPORT FileUtilities
{
public:
    static bool createIfNotExists(QString folder);
    static QStringList getFilesOfTypes(QString directory, QStringList types, bool recursive = false, bool absolutepath = false);
    static QStringList getFilesOfType(QString directory, QString type, bool recursive = false, bool absolutepath = false);
    static QString base64_encode(QString string);
    static QString base64_encode(QByteArray utf8bytes);
    static QString base64_decode(QString string);
    static QString base64_decodeFromPercentEncoding(QString string);
    static QString base64_decodeFromPercentEncoding(QByteArray utf8bytes);
    static QByteArray base64_decode(const QByteArray& in_string);
    static QByteArray base64_decode_as_bytes(QString string);
    static void CopyFolderStructure(QString src, QString dst, bool withFiles = false);
    static QStringList GetSubfolders(QString folder, bool fullpath = false, bool recursive = true);
    static QString FolderSlash(QString folder);
    static QString filename(QString file, bool withSuffix = false);
    static QString filetype(QString file);
    static qint64 filesize(QString file);
    static QString filesizeNice(QString file);
    static bool removeFile(QString file);

    static bool clearFolder(QString directory, bool deleteDirectory = false);
};

#endif // FILEUTILITIES_H
