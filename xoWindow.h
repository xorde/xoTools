#ifndef XOWINDOW_H
#define XOWINDOW_H

#include <QMainWindow>
#include "ModuleBaseAppONB.h"
#include "xotools_global.h"

class XOTOOLSSHARED_EXPORT xoWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit xoWindow(ModuleBaseAppONB *module, QWidget *parent = nullptr);
    ~xoWindow() override;

private:
    ModuleBaseAppONB *m_module = nullptr;
    QList<QMetaObject::Connection> connections;
};

#endif // XOWINDOW_H
