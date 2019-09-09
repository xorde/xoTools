#ifndef NSTACKPANEL_H
#define NSTACKPANEL_H

#include <QWidget>
#include <QGridLayout>
#include "xotools_global.h"

class XOTOOLSSHARED_EXPORT NStackPanel : public QWidget
{
Q_OBJECT
public:
    explicit NStackPanel(Qt::Orientation orientation = Qt::Horizontal, QWidget *parent = nullptr);

    ~NStackPanel();

    void add(QWidget* w);
    void setPadding(int padding);
    void setPadding(int left, int top, int right, int bottom);

    void setItemPadding(int padding);
    void setItemPadding(int left, int top, int right, int bottom);

    void setItemAlignment(QWidget *w, Qt::Alignment align);
    void setItemAlignmentByIndex(int index, Qt::Alignment align);

    NStackPanel* operator << (QWidget* w);
    void clear();
    QWidget *at(int index);
    int count() { return grid->count(); }


private:
    Qt::Orientation mOrientation;
    QGridLayout* grid = nullptr;
    QWidget* spacer = nullptr;
    QMargins m_padding = QMargins(0,0,0,0);
};

#endif // NSTACKPANEL_H
