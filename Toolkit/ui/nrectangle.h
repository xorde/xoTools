#ifndef NRECTANGLE_H
#define NRECTANGLE_H

#include <QWidget>
#include "uihelper.h"

class NRectangle : public QFrame
{
    Q_OBJECT
public:
    explicit NRectangle(QColor color, QWidget *parent = nullptr);
    NRectangle(QColor color, QColor borderColor, quint16 borderThickness, QWidget *parent = nullptr);
    NRectangle(QColor color, int w, int h, QWidget *parent = nullptr);

    void setBackground(QColor color);
    void setBorder(QColor stroke, quint16 strokeThickness);
    void setBorder(QColor stroke, quint16 left, quint16 top, quint16 right, quint16 bottom);
    void addWidget(QWidget *widget);
    QWidget* getWidget();

private:
    void paintEvent(QPaintEvent *);
    QColor _bg;
    QColor _stroke;
    quint16 _sLeft = 0;
    quint16 _sTop = 0;
    quint16 _sRight = 0;
    quint16 _sBottom = 0;
    QWidget *_innerWidget = nullptr;
    QGridLayout *_innerLayout = nullptr;
};

#endif // NRECTANGLE_H
