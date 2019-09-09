#include "nrectangle.h"
#include <QPainter>
#include <QDebug>

NRectangle::NRectangle(QColor color, QWidget *parent) : QFrame(parent)
{
    _bg = color;
}

NRectangle::NRectangle(QColor color, QColor borderColor, quint16 borderThickness, QWidget *parent) : QFrame(parent)
{
    _bg = color;
    setBorder(borderColor, borderThickness);
}

NRectangle::NRectangle(QColor color, int width, int height, QWidget *parent) : QFrame(parent)
{
    _bg = color;
    if (width > 0) setFixedWidth(width);
    if (height > 0) setFixedHeight(height);
}

void NRectangle::setBackground(QColor color)
{
    _bg = color;
    update();
}

void NRectangle::setBorder(QColor stroke, quint16 strokeThickness)
{
    _stroke = stroke;
    _sLeft = strokeThickness;
    _sTop = strokeThickness;
    _sRight = strokeThickness;
    _sBottom = strokeThickness;
    update();
}

void NRectangle::setBorder(QColor stroke, quint16 left, quint16 top, quint16 right, quint16 bottom)
{
     _stroke = stroke;
     _sLeft = left;
     _sTop = top;
     _sRight = right;
     _sBottom = bottom;
}

void NRectangle::addWidget(QWidget *widget)
{
    if (!_innerLayout)
    {
        _innerLayout = new QGridLayout(this);
        _innerLayout->setContentsMargins(0,0,0,0);
    }

    UIHelper::ClearLayout(_innerLayout);

    _innerWidget = widget;

    if (_innerWidget)
    {
        _innerLayout->addWidget(_innerWidget, 0, 0);
    }
}

void NRectangle::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    int w = width();
    int h = height();

    if (w <= 0 && h <= 0) return;

    int x = 0;
    int y = 0;

    p.setPen(Qt::NoPen);

    if (_sLeft > 0 || _sRight > 0 || _sTop > 0 || _sBottom > 0)
    {
        p.setBrush(QBrush(_stroke));
        p.drawRect(rect());

        x += _sLeft;
        y += _sTop;
        w -= (_sLeft + _sRight);
        h -= (_sTop + _sBottom);
    }

    p.setBrush(QBrush(_bg));
    p.drawRect(QRect(x, y, w, h));

    p.end();
}
