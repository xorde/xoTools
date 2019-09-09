#include "clickablewidget.h"
#include "uihelper.h"
#include <QDebug>

QColor ClickableWidget::HighlightColor = QColor("#3399ff");

ClickableWidget::ClickableWidget(QWidget *parent) : QWidget(parent)
{    
    setIsHighlighted(false);
}

void ClickableWidget::closeEvent(QCloseEvent *event)
{
    bool cancel = false;
    emit closing(cancel);

    if (cancel)
    {
        event->ignore();
        return;
    }

    emit closed();
}

void ClickableWidget::highlight()
{
    UIHelper::ChangeBackground(this, ClickableWidget::HighlightColor);
    setIsHighlighted(true);
}

void ClickableWidget::unhighlight()
{
    UIHelper::ChangeBackground(this, QColor("transparent"));
    setIsHighlighted(false);
}

void ClickableWidget::mousePressEvent(QMouseEvent *event)
{
    emit clicked(1);
    emit clickedWithPos(1, event->pos());
}

void ClickableWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit clicked(2);
    emit doubleClicked();
    emit clickedWithPos(1, event->pos());
}

void ClickableWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    emit released();

    if (ev->button() == Qt::RightButton)
    {
        emit rightClicked();
    }
}
