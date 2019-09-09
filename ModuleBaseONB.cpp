#include "ModuleBaseONB.h"

ModuleBaseONB::ModuleBaseONB(QObject* parent) : QObject(parent)
{
    m_timestampTimer.start();
}

ModuleBaseONB::~ModuleBaseONB()
{
}

QByteArray ModuleBaseONB::getIcon()
{
    return m_iconData;
}

void ModuleBaseONB::setIcon(QImage icon)
{
    QBuffer buffer(&m_iconData);
    buffer.open(QIODevice::WriteOnly);
    icon.save(&buffer, "png");
    buffer.close();
}

void ModuleBaseONB::setIcon(QString filename)
{
    QFile f(filename);
    if (f.open(QIODevice::ReadOnly))
    {
        m_iconData = f.readAll();
        f.close();
    }
}

void ModuleBaseONB::log(QString message)
{
    ONBHeader header;
    header.objectID = svcMessage;
    header.componentID = 0;
    header.svc = 1;
    header.local = 1;
    ONBPacket packet(header, message.toUtf8());
    sendPacket(packet);
}

QJsonObject ModuleBaseONB::getJsonConfig() const
{
    QJsonObject obj;
    obj["components"] = getComponentsConfigList();
    return obj;
}

QList<QJsonObject> ModuleBaseONB::getClassesInfo() const
{
    QList<QJsonObject> list;
    QList<uint32_t> keys = m_classInfo.keys();
    for (uint32_t key : keys)
    {
        ComponentBase* pComponent = m_classInfo[key];
        if (pComponent)
            list.push_back(pComponent->getJsonConfig());
    }
    return list;
}

QJsonArray ModuleBaseONB::getComponentsConfigList() const
{
    QJsonArray arr;
    for (ComponentBase* pComponent : m_components)
    {
        if (pComponent)
            arr.push_back(pComponent->getJsonConfig());
    }
    return arr;
}

bool ModuleBaseONB::isComponentExists(QString name)
{
    for (ComponentBase *c: m_components)
    {
        if (c->m_instanceName == name)
            return true;
    }
    return false;
}

bool ModuleBaseONB::newComponent(uint32_t classID, QString name)
{
    if (m_classInfo.contains(classID))
    {
        // creating component with existing name is not allowed!
        if (isComponentExists(name))
            return false;

        // create component
        ComponentBase *component = m_creators[classID](name);
        component->m_timestampTimer = &m_timestampTimer;

        // come up with unique name
        if (name.isEmpty())
        {
            int idx = 0;
            QString newName;
            do
            {
                newName = component->componentName + QString::number(++idx);
            } while (isComponentExists(newName));

            component->m_instanceName = newName;
        }
        component->m_busType = BusWebSocket;
        m_components << component;
        connect(component, SIGNAL(newData(ONBPacket)), SLOT(sendPacket(ONBPacket)));
        component->onCreate();
        component->m_created = true;
        emit componentInitialized(component->getJsonConfig());        
        return true;
    }
    return false;
}

bool ModuleBaseONB::deleteComponent(unsigned short compID)
{
    if (m_componentsMap.contains(compID))
    {
        ComponentBase *component = m_componentsMap.take(compID);
        component->onDestroy();
        m_components.removeOne(component);
        component->deleteLater(); // delete component;
        sendKillComponent(compID);
        return true;
    }
    return false;
}

//void ModuleBaseONB::connectToComponents()
//{
//    for (auto& component : m_components)
//    {
//        connect(component, SIGNAL(newData(ONBPacket)), SLOT(sendPacket(ONBPacket)));
//    }
//}

//void ModuleBaseONB::disconnectFromComponents()
//{
//    disconnect(this, SLOT(sendPacket(ONBPacket)));
//}

void ModuleBaseONB::sendKillComponent(unsigned short compID)
{
    ONBHeader header;
    header.objectID = svcKill;
    header.componentID = compID;
    header.svc = 1;
    header.local = 1;

    ONBPacket packet(header);
    sendPacket(packet);
}

void ModuleBaseONB::sendClassList()
{
    ONBHeader header;
    header.objectID = svcClass;
    header.componentID = 0;
    header.svc = 1;
    header.classInfo = 1;
    header.local = 1;

    for (unsigned short idx=0; idx<m_classes.size(); idx++)
    {
        uint32_t cid = m_classes[idx];
        m_classInfo[cid]->setId(idx+1);
        connect(m_classInfo[cid], &ComponentBase::newData, this, &ModuleBaseONB::sendClassInfoPacket);
        header.componentID = idx + 1;
        QByteArray ba(reinterpret_cast<const char*>(&cid), 4);
        ONBPacket packet(header, ba);
        sendPacket(packet);
    }

    // send dummy svcClass message meaning the end
    header.componentID = 0;
    sendPacket(ONBPacket(header));
}

void ModuleBaseONB::sendClassInfoPacket(const ONBPacket &packet)
{
    ONBHeader hdr = packet.header();
    hdr.classInfo = 1;
    sendPacket(ONBPacket(hdr, packet.data()));
}

void ModuleBaseONB::receivePacket(const ONBPacket &packet)
{
    const ONBHeader &hdr = packet.header();
    bool broadcast = !hdr.local;
    if (broadcast)
    {
        if (hdr.classInfo && hdr.objectID == aidPollClasses)
            sendClassList();
        for (auto& component : m_components)
            component->receiveData(packet);
    }
    else
    {
        unsigned short componentID = hdr.componentID;
        if (!componentID || hdr.classInfo)
            parsePacket(packet);
        else if (m_componentsMap.contains(componentID))
            m_componentsMap[componentID]->receiveData(packet);
        else // no such component
            sendKillComponent(componentID);
    }
}

void ModuleBaseONB::parsePacket(const ONBPacket &packet)
{
    const ONBHeader &hdr = packet.header();
    if (!hdr.svc) return;

    unsigned char oid = hdr.objectID;

    if (hdr.classInfo)
    {
        int idx = hdr.componentID - 1;
        if (idx < m_classes.size())
        {
            uint32_t cid = m_classes[idx];
            ComponentBase *c = m_classInfo[cid];
            c->receiveData(packet);
        }
        else
        {
            sendFail(packet);
        }
    }
    else switch(oid)
    {
      case svcIcon:
      {
        ONBHeader header;
        header.objectID = svcIcon;
        header.componentID = 0;
        header.svc = packet.header().svc;
        header.local = packet.header().local;
        ONBPacket outPacket(header, m_iconData);
        sendPacket(outPacket);
      } break;

      case svcWelcome:
      case svcWelcomeAgain:
      {
          unsigned short componentID;
          /*if (packet.data().isEmpty())
              sendModuleID();
          else*/ if (packet.data().size() == 1)
              componentID = static_cast<unsigned short>(packet.data()[0]);
          else if (packet.data().size() == 2)
              componentID = *reinterpret_cast<const unsigned short*>(packet.data().data());


          for (auto& component : m_components)
          {
              if (!component->id())
              {
                  m_componentsMap[componentID] = component;
                  component->receiveData(packet);
                  emit componentCreated(component->id());
                  break;
              }



          }
      }
      break;

      case svcCreate:
      {
        uint32_t cid = *reinterpret_cast<const uint32_t*>(packet.data().data());
        QString name = QString::fromUtf8(packet.data().data() + 4, packet.data().size() - 4);
        if (!newComponent(cid, name))
            sendFail(packet);
      }
      break;

      case svcKill:
      {
        unsigned short compID = *reinterpret_cast<const unsigned short*>(packet.data().data());
        if (!deleteComponent(compID))
            sendFail(packet);
      }
      break;
    }
}

void ModuleBaseONB::sendFail(const ONBPacket &packet)
{
    ONBHeader header;
    header.objectID = svcFail;
    header.componentID = packet.header().componentID;
    header.svc = packet.header().svc;
    header.local = packet.header().local;
    header.classInfo = packet.header().classInfo;
    QByteArray ba;
    ba.append(packet.header().objectID);
    ba.append(packet.data());
    ONBPacket outPacket(header, ba);
    sendPacket(outPacket);
}

