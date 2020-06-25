#include "ComponentBase.h"
#include <QCryptographicHash>

ComponentBase::ComponentBase(QObject *parent) : ComponentBase("GenericComponent", "", parent)
{
}

ComponentBase::ComponentBase(QString name, QString description, QObject *parent) :
    QObject(parent),
    description(description.isEmpty()? "<no description available>": description),
    serialNumber(0),
    version(reinterpret_cast<unsigned char*>(&m_version)[1]),
    versionMinor(reinterpret_cast<unsigned char*>(&m_version)[0]),
    burnCount(0),
    m_classID(0),
    m_objectCount(0),
    m_version(0),
    m_busType(0)
{
    setName(name);
    // order of service objects is predefined by ONB (to be compatible with devices)
    bindSvcObject("class", m_classID, ObjectBase::ReadOnly);
    bindSvcObject("name", m_instanceName, ObjectBase::ReadWrite); // there will be instance name in the future...
    bindSvcObject("fullName", ComponentBase::description, ObjectBase::ReadWrite);
    bindSvcObject("serial", serialNumber, ObjectBase::ReadOnly);
    bindSvcObject("version", m_version, ObjectBase::ReadOnly);
    bindSvcObject("buildDate", releaseInfo, ObjectBase::ReadOnly);
    bindSvcObject("cpuInfo", hardwareInfo, ObjectBase::ReadOnly);
    bindSvcObject("burnCount", burnCount, ObjectBase::ReadWrite);
    bindSvcObject("objCount", m_objectCount, ObjectBase::ReadOnly);
    //! @todo replace m_objectCount with m_objects.size() in the future
    bindSvcObject("busType", m_busType, ObjectBase::ReadOnly);
    bindSvcObject("className", m_componentName, ObjectBase::ReadOnly);
    bindSvcObject("icon", m_iconData, ObjectBase::ReadOnly);
}

ComponentBase::~ComponentBase()
{
    //qDebug() << "[ComponentBase] deleted" << componentName;
//    for (ObjectBase *obj: m_objects)
//        delete obj;
//    for (ObjectBase *obj: m_svcObjects)
//        delete obj;
}

uint32_t ComponentBase::timestamp() const
{
    return m_timestampTimer ? static_cast<uint32_t>(m_timestampTimer->elapsed()): 0;
}

void ComponentBase::sendObject(QString name)
{
    ObjectBase *obj = m_objectMap.value(name, nullptr);
    if (obj)
    {
        QByteArray ba;
        obj->read(ba);
        sendMessage(obj->description().id, ba);
    }
}

QJsonObject ComponentBase::getJsonConfig() const
{
    QJsonObject obj;
    obj["id"] = QString::number(m_id);
    obj["name"] = m_instanceName.value();
    obj["className"] = m_componentName.value();
    obj["descr"] = description.value();
    obj["classID"] = QString::number(m_classID);
    obj["serialNumber"] = QString::number(serialNumber);
    obj["version"] = QString::number(m_version);
    obj["releaseInfo"] = releaseInfo.value();
    obj["hardwareInfo"] = hardwareInfo.value();

    QJsonArray inputs, outputs;
    for (ObjectBase* pInfo : m_objects)
    {
        if (!pInfo)
            continue;
        switch(pInfo->flags())
        {
        case ObjectBase::Input:
            inputs.push_back(pInfo->createJsonInfo());
            break;
        case ObjectBase::Output:
            outputs.push_back(pInfo->createJsonInfo());
            break;
        default:; // to eliminate warning
        }
    }

    obj["inputs"] = inputs;
    obj["outputs"] = outputs;
    return obj;
}

void ComponentBase::setName(QString name)
{
    m_componentName = name;
    QByteArray hash = QCryptographicHash::hash(name.toUtf8(), QCryptographicHash::Md5);
    uint32_t cid = 0;
    for (int i=0; i<4; i++)
        cid ^= reinterpret_cast<uint32_t*>(hash.data())[i];
    m_classID = cid;
    //qDebug() << "HASH of" << name << "=" << cid;
}

void ComponentBase::setIcon(QImage icon)
{
    QBuffer buffer(&m_iconData);
    buffer.open(QIODevice::WriteOnly);
    icon.save(&buffer, "png");
    buffer.close();
}

void ComponentBase::setIcon(QString filename)
{
    QFile f(filename);
    if (f.open(QIODevice::ReadOnly))
    {
        m_iconData = f.readAll();
        f.close();
    }
}

bool ComponentBase::renameObject(QString oldName, QString newName)
{
    ObjectBase *obj = m_objectMap.value(oldName, nullptr);
    if (obj)
    {
        obj->m_description.name = newName;
        m_objectMap.remove(oldName);
        m_objectMap[newName] = obj;
        sendObjectInfo(obj->description().id);
        return true;
    }
    return false;
}

bool ComponentBase::deleteObject(QString name)
{
    ObjectBase *obj = m_objectMap.value(name, nullptr);
    if (obj)
    {
        m_objects.removeAt(obj->description().id);
        m_objectMap.remove(name);
        m_objectCount = m_objects.size();
        for (unsigned char oid=0; oid<m_objects.size(); oid++)
            m_objects[oid]->setId(oid);
        sendSvcObject(svcObjectCount);
        return true;
    }
    return false;
}

const ObjectBase *ComponentBase::object(QString name) const
{
    return m_objectMap.value(name, nullptr);
}

void ComponentBase::touchOutput(QString name)//, bool notifyAnyway)
{
    qDebug() << "[ComponentBase]" << name << "touched";
    ObjectBase *obj = m_objectMap.value(name, nullptr);
    if (obj)
    {
        if (!obj->m_autoPeriodMs)// || notifyAnyway)
        {
            QByteArray ba;
            obj->read(ba);
            sendMessage(obj->description().id, ba);
            qDebug() << "msg sent" << obj->name() << obj->m_autoPeriodMs;
        }
        else
        {
            obj->m_changed = true;
        }
    }
}

void ComponentBase::log(QString message)
{
    sendServiceMessage(svcMessage, message.toUtf8());
}

unsigned char ComponentBase::bindSvcObject(QString name, ObjectBase &obj, ObjectBase::Flags flags)
{
    obj.m_description.name = name;
    obj.m_description.flags |= flags;
    m_svcObjects << &obj;
    obj.setId(m_svcObjects.size() - 1);
    return obj.description().id;
}

unsigned char ComponentBase::bindObject(ObjectBase &obj)
{
    obj.m_component = this;

    m_objects << &obj;
    obj.setId(m_objects.size() - 1);
    m_objectMap[obj.name()] = &obj;

    // TODO: replace m_objectCount with m_objects.size() in the future
    m_objectCount = m_objects.size();

    // TODO: implement settings loading
//    if (obj.isStorable())
//        objnetStorage()->load(obj);

    if (m_created)
        sendSvcObject(svcObjectCount);

    return obj.description().id;
}


void ComponentBase::receiveData(const ONBPacket &packet)
{
    unsigned short compID = packet.header().componentID;
    if (compID && compID != m_id)
        return; // discard fail packet
    unsigned char oid = packet.header().objectID;
    if (packet.header().svc)
    {
        if (packet.header().local)
            parseServiceMessage(oid, packet.data());
        else
            parseGlobalServiceMessage(oid);
    }
    else
        parseMessage(oid, packet.data());
}
//---------------------------------------------------------

void ComponentBase::parseGlobalServiceMessage(unsigned char aid)
{
    switch (aid)
    {
      case aidPollNodes:
        sendServiceMessage(svcEcho);
        break;

      case aidConnReset:
        m_id = 0;
        break;
    }
}

void ComponentBase::parseServiceMessage(unsigned char oid, const QByteArray &data)
{
    switch (oid)
    {
      case svcWelcome:
      case svcWelcomeAgain:
        if (data.size() == 1)
            m_id = data[0];
        else if (data.size() == 2)
            m_id = *reinterpret_cast<const unsigned short*>(data.data());

        sendServiceMessage(svcEcho);
        break;

      case svcObjectInfo:
      {
        unsigned char _oid = data[0];
        if (_oid < m_objects.size())
        {
            sendObjectInfo(_oid);
        }
      } break;

      case svcObjectMinimum:
      case svcObjectMaximum:
      case svcObjectDefault:
      case svcObjectStep:
      case svcObjectMimeType:
      case svcObjectHint:
      case svcObjectUnit:
      case svcObjectOptions:
      case svcObjectRMIP:
      case svcObjectEnum:
      {
        unsigned char _oid = data[0];
        unsigned char metavalue = oid - svcObjectInfo;
        if (_oid < m_objects.size() && m_objects[_oid])
        {
            QByteArray ba;
            ba.append(_oid);
            ObjectBase *obj = m_objects[_oid];
            obj->readMeta(ba, static_cast<ObjectBase::MetaValue>(metavalue));
            sendServiceMessage(oid, ba);
        }
      } break;

      case svcRequestAllInfo:
        for (unsigned char _oid=0; _oid<m_svcObjects.size(); _oid++)
            sendSvcObject(_oid);
        break;

      case svcRequestObjInfo:
        for (unsigned char _oid=0; _oid<m_objects.size(); _oid++)
            sendObjectInfo(_oid);
        break;

      case svcAutoRequest:
      case svcTimedRequest:
        if (data.size())
        {
            unsigned char _oid = data[4];
            if (_oid < m_objects.size() && m_objects[_oid])
            {
                ObjectBase *obj = m_objects[_oid];
                int period = *reinterpret_cast<const int*>(data.data());
                if (period >= 0)
                    obj->m_autoPeriodMs = period;
                else
                    obj->m_autoPeriodMs = obj->m_RMIP;
                obj->m_changed = true;
                if (oid == svcTimedRequest)
                    obj->m_needTimestamp = true;

                if (obj->m_autoPeriodMs > 0)
                {
                    QTimer *timer = m_autoSendTimers.value(_oid, nullptr);
                    if (!timer)
                    {
                        timer = new QTimer();
                        m_autoSendTimers[_oid] = timer;
                        connect(timer, &QTimer::timeout, this, [_oid, this]()
                        {
                            ObjectBase *obj = this->m_objects[_oid];
                            if (obj->m_needTimestamp)
                            {
                                uint32_t timestamp = m_timestampTimer? m_timestampTimer->elapsed(): 0;
                                QByteArray ba;
                                //ba.append(reinterpret_cast<const char*>(&_oid), sizeof(unsigned char));
                                ba.append(_oid);
                                ba.append('\0'); // reserved byte
                                ba.append(reinterpret_cast<const char*>(&timestamp), sizeof(uint32_t));
                                obj->m_timestamp = timestamp;
                                objectRequestEvent(obj->name());
                                obj->read(ba);
                                this->sendServiceMessage(svcTimedObject, ba);
                            }
                            else if (obj->m_changed)
                            {
                                obj->m_changed = false;
                                objectRequestEvent(obj->name());
                                QByteArray ba;
                                obj->read(ba);
                                this->sendMessage(obj->description().id, ba);
//                                qDebug() << "sent by autorequest timer" << componentName <<"."<<obj->name();
                            }
                        });
                    }
                    timer->setInterval(obj->m_autoPeriodMs);
                    timer->start();
                }
                else
                {
                    QTimer *timer = m_autoSendTimers.value(_oid, nullptr);
                    delete timer;
//                    int period = -1;
//                    if (m_autoSendTimers.contains(_oid))
//                        period = m_autoSendTimers[_oid]->interval();
//                    QByteArray outBa;
//                    outBa.resize(4);
//                    *reinterpret_cast<int*>(outBa.data()) = period;
//                    sendServiceMessage(oid, outBa);
                }
            }
        }
        break;

      case svcUnsubscribe:
        if (data.size())
        {
            unsigned char _oid = data[0];
            if (m_autoSendTimers.contains(_oid))
                delete m_autoSendTimers.take(_oid);
        }
        else
        {
            for (QTimer *timer: m_autoSendTimers)
                delete timer;
            m_autoSendTimers.clear();
        }
        break;

      case svcTimedObject:
      {
        unsigned char _oid = data[0];
        uint32_t timestamp = *reinterpret_cast<const uint32_t*>(data.data() + 2);
        if (_oid < m_objects.size())
        {
            ObjectBase *obj = m_objects[_oid];
            if (obj)
            {
                obj->m_timestamp = timestamp;
                // TODO: get rid of copying
                parseMessage(_oid, data.mid(6));
            }
        }
      } break;

      case svcGroupedRequest:
      {
        // TODO: implement this for String type too, now it isn't working
        QByteArray ba;
        int cnt = data.size();
        for (int i=0; i<cnt; i++)
        {
            unsigned char local_oid = data[i];
            if (local_oid >= m_objects.size())
                continue;
            ObjectBase *obj = m_objects[local_oid];
            if (obj)
            {
                ba.append(local_oid);
                obj->read(ba);
            }
        }
        sendServiceMessage(svcGroupedObject, ba);
        break;
      }

      default:;
    }

    if (oid < m_svcObjects.size())
    {
        if (data.size()) // write
        {
            m_svcObjects[oid]->write(data);
        }
        else
        {
            sendSvcObject(oid);
        }
    }
}

void ComponentBase::parseMessage(unsigned char oid, const QByteArray &data)
{
    if (oid < m_objects.size())
    {
        ObjectBase *obj = m_objects[oid];
        if (!obj)
            return;
        if (data.size()) // write
        {
            obj->write(data);
            objectReceiveEvent(obj->name());
            if (obj->m_changed)
            {
                obj->m_changed = false;
                objectChangeEvent(obj->name());
            }
            // TODO: implement saving for settings objects!
//            else if (obj.isStorable())
//                objnetStorage()->save(obj);
        }
        if (!data.size())
        {
            objectRequestEvent(obj->name());
            QByteArray ba;
            obj->read(ba);
            sendMessage(oid, ba);
        }
    }

}

void ComponentBase::sendServiceMessage(unsigned char oid, const QByteArray &data)
{
    ONBHeader hdr;
    hdr.objectID = oid;
    hdr.componentID = m_id;
    hdr.svc = 1;
    hdr.local = 1;
    emit newData(ONBPacket(hdr, data));
}

void ComponentBase::sendMessage(unsigned char oid, const QByteArray &data)
{
    ONBHeader hdr;
    hdr.objectID = oid;
    hdr.componentID = m_id;
    hdr.local = 1;
    emit newData(ONBPacket(hdr, data));
}

void ComponentBase::sendSvcObject(unsigned char oid)
{
    ObjectBase *obj = m_svcObjects[oid];
    if (obj)
    {
        QByteArray ba;
        obj->read(ba);
        sendServiceMessage(oid, ba);
    }
}

void ComponentBase::sendObjectInfo(unsigned char oid)
{
    if (oid < m_objects.size() && m_objects[oid])
    {
        QByteArray ba;
        m_objects[oid]->description().read(ba);
        sendServiceMessage(svcObjectInfo, ba);
    }
}
