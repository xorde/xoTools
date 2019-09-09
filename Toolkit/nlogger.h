#ifndef NLOGGER_H
#define NLOGGER_H

#include <QObject>
#include <QMutex>
#include <QFile>

class NLogger : public QObject
{
    Q_OBJECT
public:
    explicit NLogger(QString path = "", QObject *parent = 0);
    static void Log(QString message, bool close = false);
    static void Log(QString type, QString message, bool close = false);
    static void LogTo(QString message, QString logFileName, bool close = false);
    static void LogTo(QString type, QString message, QString logFileName, bool close = false);
    static void LogToFile(QString message, QString logFileName, bool close = false);
    static void LogToFile(QString type, QString message, QString logFileName, bool close = false);
    static QString st_loggerPath;
    static void setLoggerPath(QString path);
    static QString standartLogFilePath();
public slots:
    void log(QString message, QString type = "");
private:
    QString _path;
    static FILE* pFile;
    static QByteArray file_path;
    static QMutex* st_pMutex;
};

#endif // NLOGGER_H
