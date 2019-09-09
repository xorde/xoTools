#ifndef NSCROLLVIEWER_H
#define NSCROLLVIEWER_H

#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include "nstackpanel.h"
#include <QLabel>
#include "xotools_global.h"

class XOTOOLSSHARED_EXPORT NScrollViewer : public QWidget
{
Q_OBJECT
public:
    explicit NScrollViewer(Qt::Orientation orientation = Qt::Vertical, QWidget *parent = 0);

    ~NScrollViewer();

    void addWidget(QWidget* widget);
    void add(QWidget* widget);

    void setItemPadding(int uniformPadding);
    void setItemPadding(int left, int top, int right, int bottom);

    QWidget *itemAt(int index);

    void clear();
    void setItemAlignment(QWidget *w, Qt::Alignment align);
    void setItemAlignmentByIndex(int index, Qt::Alignment align);
    void setItemVisible(int index, bool visible);

    QLabel *setPlaceholderText(const QString &text, Qt::Alignment alignment = Qt::Alignment(), Qt::Alignment textAlignment = Qt::AlignCenter, QString objectName = "NScrollViewerPlaceholder", int textContentsMargins = 16);
    void setPlaceholderWidget(QWidget *widget, Qt::Alignment alignment = Qt::Alignment());

    int count();

    void setTransparentBackground();

    void setBackground(QColor color);

    void setBackgroundAndBorder(QColor backgroundColor, quint8 borderThickness, QColor borderColor);

    QScrollArea *getScrollArea();
    NStackPanel *getStackPanel();

private:
    QScrollArea *m_scroll = nullptr;
    QGridLayout *layout = nullptr;
    NStackPanel *m_stack = nullptr;
    QWidget *m_placeholderWidget = nullptr;
    QGridLayout *m_layout = nullptr;

private slots:
    void checkPlaceholder();
};

#endif // NSCROLLVIEWER_H
