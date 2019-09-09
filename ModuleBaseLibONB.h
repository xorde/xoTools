#ifndef MODULEBASELIBONB_H
#define MODULEBASELIBONB_H

#include <QObject>
#include "ModuleBaseONB.h"
#include "xotools_global.h"
#include <QDebug>

class XOTOOLSSHARED_EXPORT ModuleBaseLibONB : public ModuleBaseONB
{
    Q_OBJECT
public:
    ModuleBaseLibONB();
    virtual bool isActive() override;
protected:
    virtual void sendPacket(const ONBPacket &packet) override;
    virtual bool start() override;
signals:
    void newPacket(const ONBPacket &packet);
};

#endif // MODULEBASELIBONB_H
