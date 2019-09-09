#include "NPushButton.h"
#include "helpers/CursorHelper.h"

NPushButton::NPushButton(QWidget *parent) : QPushButton(parent)
{
    installEventFilter(this);
    CursorHelper::setCursor(this, Qt::PointingHandCursor);
}

NPushButton::NPushButton(const QString &text, QWidget *parent) : QPushButton(text, parent)
{
    installEventFilter(this);
    CursorHelper::setCursor(this, Qt::PointingHandCursor);
}

NPushButton::NPushButton(const QIcon &icon, const QString &text, const QString &objectName, QWidget *parent) : QPushButton(icon, text, parent)
{
    installEventFilter(this);
    CursorHelper::setCursor(this, Qt::PointingHandCursor);
    if (!objectName.isEmpty()) setObjectName(objectName);
}

NPushButton::~NPushButton()
{
    removeEventFilter(this);
}

void NPushButton::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
    emit onHidden();
    emit onVisibleChanged(false);
}

void NPushButton::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    emit onShown();
    emit onVisibleChanged(true);
}

void NPushButton::disable()
{
    if (!isEnabled()) return;
    setEnabled(false);
    emit disabled();
    emit enabledStateChanged(false);
}

void NPushButton::enable()
{
    if (isEnabled()) return;
    setEnabled(true);
    emit enabled();
    emit enabledStateChanged(true);
}

void NPushButton::setEnabledState(bool enabled)
{
    if (enabled) enable();
    else disable();
}

#include <QDebug>
bool NPushButton::eventFilter(QObject *, QEvent *event)
{
    auto type = event->type();
    switch(type)
    {
    case QEvent::Enter:
        if (isEnabled())
        {
            m_hovered = true;
            emit mouseHover();
            emit mouseHoverChanged(m_hovered);
        }
        break;

    case QEvent::Leave:
        if (isEnabled())
        {
            m_hovered = false;
            emit mouseLeft();
            emit mouseHoverChanged(m_hovered);
        }
        break;



//    case QMouseEvent::MouseButtonDblClick:
//        emit doubleClicked();
//        break;
    }
    return false;
}

void NPushButton::mouseDoubleClickEvent(QMouseEvent *)
{
    emit doubleClicked();
}
