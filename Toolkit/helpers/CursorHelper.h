#ifndef CURSORHELPER_H
#define CURSORHELPER_H

#include <QObject>
#include <QCursor>
#include <QMap>
#include <QWidget>

class CursorHelper : public QObject
{
    Q_OBJECT
public:
    static void InstallCursor(Qt::CursorShape shape, QPixmap image, int x, int y);

    static void setIsCustomCursorEnabled(bool enabled);

    static bool getIsCustomCursorEnabled();

    static void setCursor(QWidget *widget, Qt::CursorShape shape);

    static bool m_isCustomCursorEnabled;
    static QMap<QWidget*, Qt::CursorShape> m_widgets;
    static QMap<Qt::CursorShape, QCursor> m_customCursors;
    static void reassign();
};

#endif // CURSORHELPER_H
