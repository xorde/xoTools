#include "ModuleBaseLibONB.h"

ModuleBaseLibONB::ModuleBaseLibONB()
{

}

bool ModuleBaseLibONB::isActive()
{
    return true;
}

bool ModuleBaseLibONB::start()
{
    return true;
}

void ModuleBaseLibONB::sendPacket(const ONBPacket &packet)
{
    emit newPacket(packet);
}
