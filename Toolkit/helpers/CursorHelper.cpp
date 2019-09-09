#include "CursorHelper.h"

bool CursorHelper::m_isCustomCursorEnabled = false;

QMap<QWidget*, Qt::CursorShape> CursorHelper::m_widgets = QMap<QWidget*, Qt::CursorShape>();

QMap<Qt::CursorShape, QCursor> CursorHelper::m_customCursors = QMap<Qt::CursorShape, QCursor>();

void CursorHelper::InstallCursor(Qt::CursorShape shape, QPixmap image, int x, int y)
{
    m_customCursors.insert(shape, QCursor(image, x, y));
    reassign();
}

void CursorHelper::setIsCustomCursorEnabled(bool enabled)
{
    if (m_isCustomCursorEnabled == enabled) return;

    m_isCustomCursorEnabled = enabled;

    reassign();
}

bool CursorHelper::getIsCustomCursorEnabled() { return m_isCustomCursorEnabled; }

void CursorHelper::setCursor(QWidget *widget, Qt::CursorShape shape)
{
    if (!widget) return;
    if (m_isCustomCursorEnabled && m_customCursors.contains(shape))
    {
        widget->setCursor(m_customCursors[shape]);
    }
    else
    {
        widget->setCursor(QCursor(shape));
    }

    QObject::connect(widget, &QWidget::destroyed, [=]() { m_widgets.remove(widget); });
    m_widgets.insert(widget, shape);
}

void CursorHelper::reassign()
{
    foreach (QWidget *widget, m_widgets.keys())
    {
        auto shape = m_widgets[widget];
        setCursor(widget, shape);
    }
}
