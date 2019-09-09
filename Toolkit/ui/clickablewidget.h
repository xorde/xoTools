#ifndef CLICKABLEWIDGET_H
#define CLICKABLEWIDGET_H

#include <QWidget>
#include <QMouseEvent>

class ClickableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ClickableWidget(QWidget *parent = 0);

    static QColor HighlightColor;

public:
    Q_PROPERTY(bool IsHighlighted MEMBER mIsHighlighted READ getIsHighlighted WRITE setIsHighlighted NOTIFY changedIsHighlighted)
    Q_INVOKABLE bool getIsHighlighted() { return mIsHighlighted; }
    Q_INVOKABLE void setIsHighlighted(bool value)
    {
        if (value == mIsHighlighted) return;
        mIsHighlighted = value;
        if (value) { highlight(); }
        else { unhighlight(); }
        emit changedIsHighlighted(value);
    }
signals:
    Q_SIGNAL void changedIsHighlighted(bool value);
private:

    bool mIsHighlighted;
    void closeEvent(QCloseEvent *event);

public slots:
    void highlight();
    void unhighlight();

signals:
    void clicked(int clickCount);
    void clickedWithPos(int clickCount, QPoint pos);
    void doubleClicked();
    void released();
    void rightClicked();
    void closing(bool &cancel);
    void closed();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *) override;

};

#endif // CLICKABLEWIDGET_H
