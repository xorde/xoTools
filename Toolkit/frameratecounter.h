#ifndef FRAMERATECOUNTER
#define FRAMERATECOUNTER

#include <QObject>
#include <QDateTime>
#include "xotools_global.h"

class XOTOOLSSHARED_EXPORT FrameRateCounter : public QObject
{
Q_OBJECT
public:
    explicit FrameRateCounter(QObject *parent = nullptr) : QObject(parent)
    {
        restart();
    }

    ~FrameRateCounter()
    {

    }

    Q_INVOKABLE void restart()
    {
        m_frameCount = 0;
        m_checkPoint = QDateTime::currentDateTime();
        setFrameRate(0);
    }

    Q_INVOKABLE void nextFrame()
    {
        m_frameCount++;
        QDateTime cur = QDateTime::currentDateTime();        
        if (m_dates.count() > m_frameAccumulator)        
            m_dates.removeAt(0);        

        m_dates.append(cur.toMSecsSinceEpoch());
        auto dif = m_dates.last() - m_dates.first();
        if (dif == 0)
            dif = 1;

        setFrameRate(1000.0*(m_dates.count()-1)/dif);
    }

public:
    Q_PROPERTY(double frameRate READ getFrameRate WRITE setFrameRate MEMBER m_frameRate NOTIFY frameRateChanged)
    Q_INVOKABLE double getFrameRate() { return m_frameRate; }
    Q_INVOKABLE void setFrameRate(double value)
    {
        m_frameRate = value;
        emit frameRateChanged(m_frameRate);
    }
    void setAccumulator(int count)
    {
        if (count < 1) count =1;
        m_frameAccumulator = count;
    }

signals:
    void frameRateChanged(double value);

private:
    double m_frameRate = 0;
    int m_frameCount;
    QDateTime m_checkPoint;
    QList<qint64> m_dates;
    int m_frameAccumulator = 10;

};

#endif // FRAMERATECOUNTER

