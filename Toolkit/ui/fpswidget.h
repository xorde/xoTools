#ifndef FPSWIDGET2_H
#define FPSWIDGET2_H

#include <QWidget>
#include "frameratecounter.h"
#include <QLabel>
#include "xotools_global.h"

class XOTOOLSSHARED_EXPORT FPSWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FPSWidget(FrameRateCounter *counter = nullptr, QWidget *parent = nullptr);
    ~FPSWidget();
    Q_PROPERTY(FrameRateCounter* counter READ getCounter WRITE setCounter MEMBER m_counter NOTIFY counterChanged)
    Q_INVOKABLE void setCounter(FrameRateCounter* value);
    Q_INVOKABLE FrameRateCounter* getCounter();

    void setSuffix(QString suffix);

signals:
    Q_SIGNAL void counterChanged(FrameRateCounter* value);
private:
    FrameRateCounter* m_counter = nullptr;
    QLabel *m_txt;
    QString m_suffix = "";

public slots:
    void frameRateChanged(double value);
};

#endif // FPSWIDGET_H
