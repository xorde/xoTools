#ifndef GLOBALCONSOLE_H
#define GLOBALCONSOLE_H

#include <QObject>
#include <QDateTime>
#include <QCoreApplication>
#include <QDebug>

#ifdef VLD
#include <vld.h>
#endif
#include "xotools_global.h"

class XOTOOLSSHARED_EXPORT GlobalConsole : public QObject
{
Q_OBJECT
    struct AverageCounter
    {
        int count = 0;
        double value = 0;
        int countMax = 5;
    };

    GlobalConsole(QObject *parent = nullptr);
    ~GlobalConsole();

    GlobalConsole(GlobalConsole* in_pGlobalConsole) = delete;
    void operator = (GlobalConsole* in_pGlobalConsole) = delete;

    static GlobalConsole* m_pInstance;
    QHash<QString, QStringList> entriesByType;

public:
    static bool FullOptimization;

    static GlobalConsole* Instance();
    static void writeItem(const QString &message, const QString &type = QString(), const QDateTime &date = QDateTime::currentDateTime());
    static void writeLine(QString text, QDateTime time = QDateTime::currentDateTime());
    static QString currentText();

    static void enableLogging(bool enable = true, QString fileName = QString());

    static double countAverage(QString name, double value, int countMax = 5);
    static double getAverage(QString name);
    static QStringList getAverageNames();
    static void setStatus(QString name, QString value);
    static QString getStatus(QString name);
    static void clearStatus(QString name = "");
    static QStringList getStatusNames();

    static void disableType(QString type, bool disabled = true);

    QString _currentText();
    void freeData();

    double _countAverage(QString &name, double value, int countMax = 5);
    double _getAverage(QString &name);
    QStringList _getAverageNames();
    void _setStatus(QString name, QString value);
    QString _getStatus(QString name);
    void _clearStatus(QString name = "");
    QStringList _getStatusNames();

    void _enableLogging(bool enable = true, QString fileName = QString());


signals:
    void newEntry(QString entry);

    void newAverageName(QString name);
    void newStatusName(QString name);

    void updatedAverage(QString name, double value);
    void updatedStatus(QString name, QString value);
    void statusCleared(QString name);

public slots:
    void _writeItem(QString message, const QString &type = QString(), const QDateTime &date = QDateTime::currentDateTime());
    void _writeLine(QString text, QDateTime date = QDateTime::currentDateTime());
    void _disableType(QString type, bool disabled = true);

private:
    QMap<QString, AverageCounter> m_counters;
    QMap<QString, QString> m_statuses;

    QString m_logFileName = "";
    bool m_logEnabled = false;

    QStringList m_disabledTypes;
};

class GlobalConsoleWriter
{
public:
    GlobalConsoleWriter();
    ~GlobalConsoleWriter();

    GlobalConsoleWriter& operator << (int in_int);
    GlobalConsoleWriter& operator << (short in_shrt);
    GlobalConsoleWriter& operator << (char in_char);
    GlobalConsoleWriter& operator << (QString in_str);
    GlobalConsoleWriter& operator << (char* in_char);
    GlobalConsoleWriter& operator << (float in_flt);
    GlobalConsoleWriter& operator << (double in_dbl);
};

GlobalConsoleWriter &ntDebug();

#endif // GLOBALCONSOLE_H
