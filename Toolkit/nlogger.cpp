#include "nlogger.h"
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDateTime>
#include <QDebug>
#include "texthelper.h"

FILE* NLogger::pFile = nullptr;
QByteArray NLogger::file_path;
QString NLogger::st_loggerPath = "";
QMutex *NLogger::st_pMutex = new QMutex();

NLogger::NLogger(QString path, QObject *parent) : QObject(parent)
{
    if (path.length() > 0)
    {
        _path = path;
    }
    else
    {
        _path = standartLogFilePath();
    }
}

void NLogger::Log(QString message, bool close)
{
    LogTo(message, st_loggerPath, close);
}

void NLogger::Log(QString type, QString message, bool close)
{
    LogTo(type, message, st_loggerPath, close);
}

void NLogger::LogTo(QString type, QString message, QString logFileName, bool close)
{
    LogTo((type.length() > 0 ? type + "\t" : "") + message, logFileName, close);
}

void NLogger::LogToFile(QString type, QString message, QString logFileName, bool close)
{
    LogToFile((type.isEmpty() ? "" : type + "\t") + message, logFileName, close);
}

void NLogger::LogToFile(QString message, QString logFileName, bool /*close*/)
{    
    QMutexLocker ml(st_pMutex);
    if (file_path.isEmpty())
        file_path = logFileName.toUtf8();

    pFile = fopen(file_path.data(), "a");
    if (pFile)
    {
        QByteArray data(QString(QDateTime::currentDateTime().toString("hh:mm:ss.zzz") + "\t" + message + "\r\n").toUtf8());
        unsigned int sz = static_cast<unsigned int>(data.size());
        fwrite(data.data(), sizeof(char), sz, pFile);
        fclose (pFile);
    }
}

void NLogger::LogTo(QString message, QString logFileName, bool close)
{
    QDateTime d = QDateTime::currentDateTime();
    QString path = logFileName+d.toString("yyyy.MM.dd")+".log";

    LogToFile(message, path, close);
}

void NLogger::setLoggerPath(QString path)
{
    st_loggerPath = path;
}

QString NLogger::standartLogFilePath()
{
    return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + QCoreApplication::applicationName() + "_" + TextHelper::DateTimeFileString() + ".log";
}

void NLogger::log(QString message, QString type)
{
    LogToFile(type, message);
}
