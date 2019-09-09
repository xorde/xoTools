#ifndef NPUSHBUTTON_H
#define NPUSHBUTTON_H

#include <QWidget>
#include <QEvent>
#include <QMouseEvent>
#include <QPushButton>
#include "xotools_global.h"

class XOTOOLSSHARED_EXPORT NPushButton : public QPushButton
{
Q_OBJECT
public:
    explicit NPushButton(QWidget *parent = nullptr);
    explicit NPushButton(const QString &text, QWidget *parent = Q_NULLPTR);
    NPushButton(const QIcon& icon, const QString &text, QWidget *parent = Q_NULLPTR);
    NPushButton(const QIcon& icon, const QString &text,  const QString &objectName, QWidget *parent = Q_NULLPTR);

    ~NPushButton();
    bool isHovered() { return m_hovered; }

    void setInteractable(bool interactable);

    void hideEvent(QHideEvent *event);
    void showEvent(QShowEvent *event);

signals:
    void mouseHover();
    void mouseLeft();
    void mouseHoverChanged(bool hovered);
    void disabled();
    void enabled();
    void enabledStateChanged(bool enabled);
    void doubleClicked();

    void onHidden();
    void onShown();
    void onVisibleChanged(bool visible);



public slots:
    void enable();
    void disable();
    void setEnabledState(bool enabled);

protected:
    bool eventFilter(QObject *, QEvent *event);
    bool m_hovered = false;

    void mouseDoubleClickEvent(QMouseEvent *);
};

#endif // NPUSHBUTTON_H
