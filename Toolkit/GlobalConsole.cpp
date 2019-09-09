#include "GlobalConsole.h"
#include "texthelper.h"
#include "nlogger.h"

static GlobalConsoleWriter st_GCW;

bool GlobalConsole::FullOptimization = false;

GlobalConsole *GlobalConsole::m_pInstance = nullptr;

GlobalConsole::GlobalConsole(QObject *parent) : QObject(parent)
{

}

GlobalConsole::~GlobalConsole()
{

}

GlobalConsole *GlobalConsole::Instance()
{
    if (m_pInstance == nullptr)
        m_pInstance = new GlobalConsole();
    return m_pInstance;
}

void GlobalConsole::freeData()
{
    if (m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

double GlobalConsole::_countAverage(QString &name, double value, int countMax)
{
    if (FullOptimization) return 0;

    bool exists = m_counters.contains(name);

    AverageCounter cnt;
    if (exists)
    {
        cnt = m_counters[name];
    }

    cnt.countMax = qMax(1, countMax);

    int count = cnt.count;
    double val = cnt.value;
    if (count > 0)
    {
        val = (val*(count-1) + value)/count;
        cnt.count ++;
        if (cnt.count > cnt.countMax) cnt.count = cnt.countMax;
    }
    else
    {
        val = value;
        cnt.count = 1;
    }

    cnt.value = val;
    m_counters.insert(name, cnt);

    emit updatedAverage(name, val);

    if (!exists) emit newAverageName(name);

    return val;
}

double GlobalConsole::_getAverage(QString &name)
{
    if (!m_counters.contains(name)) return 0;
    return m_counters[name].value;
}

QStringList GlobalConsole::_getAverageNames()
{
    return m_counters.keys();
}

void GlobalConsole::_setStatus(QString name, QString value)
{
    if (FullOptimization) return;

    bool exists = m_statuses.contains(name);

    m_statuses.insert(name, value);
    emit updatedStatus(name, value);

    if (!exists) emit newStatusName(name);
}

QString GlobalConsole::_getStatus(QString name)
{
    if (m_statuses.contains(name))
    {
        return m_statuses[name];
    }
    return "";
}

void GlobalConsole::_clearStatus(QString name)
{
    if (FullOptimization) return;
    if (name.length() == 0)
    {
        m_statuses.clear();
        emit statusCleared("");
    }
    else
    {
        m_statuses.remove(name);
        emit statusCleared(name);
    }
}

QStringList GlobalConsole::_getStatusNames()
{
    return m_statuses.keys();
}

void GlobalConsole::_enableLogging(bool enable, QString fileName)
{
    if (fileName.length() == 0) fileName = NLogger::standartLogFilePath();

    m_logEnabled = enable;
    m_logFileName = fileName;
}

void GlobalConsole::writeItem(const QString& message, const QString& type, const QDateTime& date)
{
    Instance()->_writeItem(message, type, date);
}

void GlobalConsole::writeLine(QString text, QDateTime time)
{
    Instance()->_writeLine(text, time);
}

QString GlobalConsole::currentText()
{
    return Instance()->_currentText();
}

void GlobalConsole::enableLogging(bool enable, QString fileName)
{
    Instance()->_enableLogging(enable, fileName);
}

double GlobalConsole::countAverage(QString name, double value, int countMax)
{
    return Instance()->_countAverage(name, value, countMax);
}

double GlobalConsole::getAverage(QString name)
{
    return Instance()->_getAverage(name);
}

QStringList GlobalConsole::getAverageNames()
{
    return Instance()->_getAverageNames();
}

void GlobalConsole::setStatus(QString name, QString value)
{
    Instance()->_setStatus(name, value);
}

QString GlobalConsole::getStatus(QString name)
{
    return Instance()->_getStatus(name);
}

void GlobalConsole::clearStatus(QString name)
{
    Instance()->_clearStatus(name);
}

QStringList GlobalConsole::getStatusNames()
{
    return Instance()->_getStatusNames();
}

void GlobalConsole::disableType(QString type, bool disabled)
{
    Instance()->_disableType(type, disabled);
}

QString GlobalConsole::_currentText()
{
    QString result;
    for(const auto& key : entriesByType.keys())
        result += entriesByType[key].join("\n");
    return result;
}

void GlobalConsole::_writeItem(QString message, const QString& type, const QDateTime& date)
{
    if (FullOptimization)
        return;

    if (!type.isEmpty() && m_disabledTypes.contains(type, Qt::CaseInsensitive))
    {
        return;
    }

    entriesByType[type].prepend(message);
    if (m_logEnabled)
    {
        NLogger::LogToFile(type, message, m_logFileName, true);
    }

    if (!type.isEmpty())
    {
        message.prepend(type+" ");
    }

    message.prepend(date.toString("hh:mm:ss.zzz")+" ");
    emit newEntry(message);
    qDebug() << message;
}

void GlobalConsole::_writeLine(QString text, QDateTime date)
{
    _writeItem(text, "", date);
}

void GlobalConsole::_disableType(QString type, bool disabled)
{
    int index = m_disabledTypes.indexOf(type);
    if (disabled)
    {
        if (index < 0)
        {
            m_disabledTypes.append(type);
        }
    }
    else if (index >= 0)
    {
        m_disabledTypes.removeAt(index);
    }
}

GlobalConsoleWriter& ntDebug()
{
    return st_GCW;
}

GlobalConsoleWriter::GlobalConsoleWriter()
{

}

GlobalConsoleWriter::~GlobalConsoleWriter()
{

}

GlobalConsoleWriter &GlobalConsoleWriter::operator <<(int in_int)
{
    QString data = QString::number(in_int);
    GlobalConsole::Instance()->writeItem(data);
    return st_GCW;
}

GlobalConsoleWriter &GlobalConsoleWriter::operator <<(short in_shrt)
{
    QString data = QString::number(in_shrt);
    GlobalConsole::Instance()->writeItem(data);
    return st_GCW;
}

GlobalConsoleWriter &GlobalConsoleWriter::operator <<(char in_char)
{
    QString data = QString::number(in_char);
    GlobalConsole::Instance()->writeItem(data);
    return st_GCW;
}

GlobalConsoleWriter& GlobalConsoleWriter::operator << (QString in_str)
{
    GlobalConsole::Instance()->writeItem(in_str);
    return st_GCW;
}

GlobalConsoleWriter& GlobalConsoleWriter::operator << (char* in_char)
{
    QString data(in_char);
    GlobalConsole::Instance()->writeItem(data);
    return st_GCW;
}

GlobalConsoleWriter &GlobalConsoleWriter::operator <<(float in_flt)
{
    QString data = QString::number(in_flt);
    GlobalConsole::Instance()->writeItem(data);
    return st_GCW;
}

GlobalConsoleWriter &GlobalConsoleWriter::operator <<(double in_dbl)
{
    QString data = QString::number(in_dbl);
    GlobalConsole::Instance()->writeItem(data);
    return st_GCW;
}
