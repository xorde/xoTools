#include "fileutilities.h"
#include <QDebug>
#include <QCoreApplication>

QString FileUtilities::base64_encode(QString string)
{
    QByteArray ba;
    ba.append(string);
    return ba.toBase64();
}

QString FileUtilities::base64_encode(QByteArray utf8bytes)
{
    return utf8bytes.toBase64();
}

QByteArray FileUtilities::base64_decode(const QByteArray& in_string)
{
    return QByteArray::fromBase64(in_string);
}

QString FileUtilities::base64_decode(QString string)
{
    QByteArray ba;
    ba.append(string);
    return QByteArray::fromBase64(ba);
}

QString FileUtilities::base64_decodeFromPercentEncoding(QString string)
{
    string.replace("%3D", "=");
    return base64_decode(string);
}

QString FileUtilities::base64_decodeFromPercentEncoding(QByteArray utf8bytes)
{
    return base64_decodeFromPercentEncoding(QString::fromUtf8(utf8bytes));
}

QByteArray FileUtilities::base64_decode_as_bytes(QString string)
{
    QByteArray ba;
    ba.append(string);
    return QByteArray::fromBase64(ba);
}

void FileUtilities::CopyFolderStructure(QString src, QString dst, bool withFiles)
{    
    src = FolderSlash(src);
    dst = FolderSlash(dst);
    createIfNotExists(dst);
    QStringList folders = GetSubfolders(src);
    foreach (QString folder, folders)
    {
        QString newfolder = dst + folder;
        createIfNotExists(newfolder);
    }

    if (withFiles)
    {
        auto files = FileUtilities::getFilesOfType(src, "", true, true);
        foreach (auto file, files)
        {
            auto newFile = file;
            newFile.replace(src, dst);
            QFile::copy(file, newFile);
        }
    }
}

QStringList FileUtilities::GetSubfolders(QString folder, bool fullpath, bool recursive)
{
    folder = FolderSlash(folder);
    QStringList folders;
    QDir f(folder);
    f.setFilter(QDir::Dirs);

    QStringList ff = f.entryList();
    foreach (QString sub, ff)
    {
        if (sub.startsWith("."))
            continue;

        sub = FolderSlash(sub);
        if (fullpath)
            folders << folder + sub;
        else
            folders << sub;

        if (recursive)
        {
            QStringList subsubs = GetSubfolders(folder + sub);
            foreach (QString subsub, subsubs)
            {
                if (fullpath) folders << folder + sub + subsub;
                else folders << sub + subsub;
            }
        }
    }
    return folders;
}


QString FileUtilities::FolderSlash(QString folder)
{
    if (folder.length() > 0)
    {
        folder = folder.replace('\\', "/");
        if (!folder.endsWith('/'))
            folder += "/";
    }
    return folder;
}

QString FileUtilities::filename(QString file, bool withSuffix)
{
    QFileInfo f(file);
    QString fn = f.fileName();
    if (!withSuffix)
    {
        QString suffix = f.suffix();
        if (suffix.length() > 0)
            return fn.left(fn.length() - 1 - suffix.length());
    }
    return fn;
}

QString FileUtilities::filetype(QString file)
{
    QFileInfo f(file);
    return f.suffix();
}

QString FileUtilities::filesizeNice(QString file)
{
    QString r;
    QFileInfo f(file);
    if (f.exists())
    {
        double s = f.size();
        int order = 0;
        while (s > 1024)
        {
            s = s/1024;
            order ++;
        }
        switch(order)
        {
        case 0: r = QCoreApplication::translate("Filesize", "B"); break;
        case 1: r = QCoreApplication::translate("Filesize", "KB"); break;
        case 2: r = QCoreApplication::translate("Filesize", "MB"); break;
        case 3: r = QCoreApplication::translate("Filesize", "GB"); break;
        case 4: r = QCoreApplication::translate("Filesize", "TB"); break;
        }
        if (s < 1024)
            r = QString::number(s, 'f', order == 0 ? 0 : 2) + " " + r;
    }
    return r;
}

bool FileUtilities::removeFile(QString file)
{
    QFile f(file);
    if (f.exists())
    {
        f.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
        bool result = f.remove();

        if (!result)
        {
            qDebug() << "REMOVE BAD" << file << f.errorString();
        }

        return result;
    }
    return false;
}

bool FileUtilities::clearFolder(QString directory, bool deleteDirectory)
{
    QDir d(directory);
    if (d.exists())
    {
        bool removed = d.removeRecursively();

        if (!deleteDirectory)
        {
            qDebug() << "Creating dir" << directory;
            createIfNotExists(directory);
        }

        return removed;
    }

    return false;
}

qint64 FileUtilities::filesize(QString file)
{
    qint64 size = 0;
    if (QFile::exists(file))
    {
        QFileInfo f(file);
        size = f.size();
    }
    return size;
}

bool FileUtilities::createIfNotExists(QString folder)
{
    QDir d;
    if (!d.exists(folder))
        d.mkpath(folder);

    return d.exists(folder);
}

QStringList FileUtilities::getFilesOfTypes(QString directory, QStringList types, bool recursive, bool absolutepath)
{
    QStringList files;
    QDir dir(directory);
    if (!dir.exists())
        return files;

    QString absdir = dir.absolutePath();

    if (types.count() > 0)
    {
        QStringList filters;
        foreach(QString type, types)
        {
            if (type.isEmpty()) continue;
            QString t = type.replace(".", "").replace("*", "");
            filters << "*."+t;
        }

        if (filters.count() > 0)
            dir.setNameFilters(filters);
    }
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    QStringList fileList = dir.entryList();
    for (int i=0; i<fileList.count(); i++)
    {
        QString f = fileList[i];
        if (absolutepath)
            f = absdir + "/" + f;
        files << f;
    }
    if (recursive)
    {
        dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        QStringList dirList = dir.entryList();
        for (int i = 0, sz = dirList.size(); i < sz; ++i)
        {
            QString subdir = dirList.at(i);
            QString newPath = QString("%1/%2").arg(absdir).arg(subdir);
            QStringList innerFiles = getFilesOfTypes(newPath, types, recursive, false);

            foreach (QString innerFile, innerFiles)
            {
                files << ((absolutepath ? absdir + "/" : "") + subdir + "/" + innerFile);
            }
        }
    }

    return files;
}

QStringList FileUtilities::getFilesOfType(QString directory, QString type, bool recursive, bool absolutepath)
{
    QStringList types;
    types << type;
    return getFilesOfTypes(directory, types, recursive, absolutepath);
}
