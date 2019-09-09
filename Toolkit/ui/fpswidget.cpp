#include "fpswidget.h"
#include <QDebug>
#include <QGridLayout>

FPSWidget::FPSWidget(FrameRateCounter *counter, QWidget *parent) : QWidget(parent)
{
    QGridLayout *mainlay = new QGridLayout;
    setLayout(mainlay);
    m_txt = new QLabel;
    mainlay->addWidget(m_txt, 0, 0, Qt::AlignRight);

    setCounter(counter);

    frameRateChanged(0.0);
}

FPSWidget::~FPSWidget()
{

}

void FPSWidget::setCounter(FrameRateCounter *value)
{
    if (m_counter)
    disconnect(m_counter, SIGNAL(frameRateChanged(double)), this, SLOT(frameRateChanged(double)));

    m_counter = value;
    emit counterChanged(m_counter);

    if (m_counter)
    connect(m_counter, SIGNAL(frameRateChanged(double)), this, SLOT(frameRateChanged(double)));
}

void FPSWidget::frameRateChanged(double value)
{
    if (!isVisible())
        return;

    QString f = QString::number(value, 'f', 1);
    m_txt->setText(f + m_suffix);
}

FrameRateCounter *FPSWidget::getCounter()
{
    return m_counter;
}

void FPSWidget::setSuffix(QString suffix)
{
    m_suffix = suffix;
}


