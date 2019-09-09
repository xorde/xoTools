#ifndef GLOBALCONSOLEWIDGET_H
#define GLOBALCONSOLEWIDGET_H

#include <QWidget>
#include <QPlainTextEdit>
#include "GlobalConsole.h"
#include "xotools_global.h"

class XOTOOLSSHARED_EXPORT GlobalConsoleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GlobalConsoleWidget(QWidget *parent = nullptr);
public slots:
    void newEntry(QString text);
    void clear();
private:
    QPlainTextEdit *m_uiText = nullptr;
};

#endif // GLOBALCONSOLEWIDGET_H
