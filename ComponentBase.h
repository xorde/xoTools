#ifndef COMPONENTBASE_H
#define COMPONENTBASE_H

#include <QJsonArray>
#include <QJsonObject>
#include <QImage>
#include <QBuffer>
#include <QFile>
#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include "Protocol/xoTypes.h"
#include "Protocol/ObjectInfo.h"
#include "Protocol/ONBPacket.h"
#include "xotools_global.h"
#include <QDebug>
#include <cmath>

//! @brief The base for the ONB component.
//!
//! To create the component you should derive from this class.
//! Declare the variables to be bound as members of your class.
//! The component description is located in the constructor.
//! There you must complete protected fields with necessary values.
//! The required fields are @ref componentName and @ref classID.
//! Then bind the variables by means of createInput(), createOutput() or createSetting(),
//! set the meta-values with desired info, see ONBObject.
//! Variables must be allocated if they are pointers or items of a container.
//!
//! @attention Don't perform any initialization of your logic in the constructor!
//!
//! Your component should reimplement the virtual methods onCreate(), onDestroy()
//! to initialize and terminate respectively the component logic
//! (e.g. run/stop timers, threads).
//!
//! Object flow is controlled by following methods:
//! to issue an object use sendObject(), touchOutput();
//! when an object or object request is received, the virtual methods are called:
//! objectReceiveEvent(), objectChangeEvent(), objectRequestEvent().
//!
//! The component is able to change its objects at run-time.
//! Just use createInput(), createOutput(), createSetting() during the run-time.
//! Also you can use renameObject() and deleteObject() to modify the component appearance.
//! To modify the object itself use rebindObject(), this method allows you to bind
//! the existing object to another variable. Changing the meta-values associated with the object
//! is not supported yet.
//! @see ONBObject
//!
//! Here is an example of ONB component implementing Gain function.
//! \n \b Gain.h
//! @code
//! #include "ComponentBase.h"
//! class Gain : public ComponentBase
//! {
//! public:
//!     Gain();
//! protected:
//!     void onCreate() override;
//!     void onDestroy() override;
//!     void objectReceiveEvent(QString name) override;
//! private:
//!     float value = 0;
//!     float result = 0;
//!     float gain = 1.0f;
//! };
//! @endcode
//! \b Gain.cpp
//! @code
//! #include "Gain.h"
//! Gain::Gain() :
//!     ComponentBase("Gain")
//! {
//!     description = "Basic amplifier with specified gain";
//!     version = 1;
//!     createInput("value", value);
//!     createOutput("result", result);
//!     createSetting("gain", gain).def(1.0f);
//! }
//!
//! void Gain::objectReceiveEvent(QString name)
//! {
//!     if (name == "value")
//!     {
//!         result = value * gain;
//!         touchOutput("result");
//!     }
//! }
//!
//! void Gain::onCreate()
//! {
//!     // do_a_barrel_roll();
//! }
//!
//! void Gain::onDestroy()
//! {
//!     // rollback();
//! }
//! @endcode
class XOTOOLSSHARED_EXPORT ComponentBase : public QObject
{
    Q_OBJECT

public:
    //! Creates component and binds service objects.
    explicit ComponentBase(QObject *parent = nullptr);

    //! Construct the component with given name and description.
    ComponentBase(QString name, QString description = QString(), QObject *parent = nullptr);

    //! Deletes all its objects, including service objects.
    virtual ~ComponentBase();

    //! @brief Get componentID of the component.
    //! @details After connection to the core, the componentID is assigned to the component.
    //! @return Assigned componentID.
    unsigned short id() const {return m_id;}

    //! @brief Get current timestamp.
    //! @details The timestamp is measured in milliseconds since system start.
    //! Synchronization procedure between components is planned but not implemented yet.
    //! @return Current timestamp value in milliseconds.
    uint32_t timestamp() const;

    //! wut? whatta hell I see?? it is work for the core!!
    QJsonObject getJsonConfig() const;

signals:
    //! @brief [DEPRECATED]
    //! @deprecated
    //! Use objectChangeEvent() instead!
    void objectChanged(QString name);

protected:
    //! Set the name of the component. Used to name the type, must be unique.
    void setName(QString name);
//    //! The name of the component. Used to name the type, must be unique.
//    QString componentName;
//    //! ClassID of the component. Class identifier, must be unique. There is classification table (must be somewhere).
//    uint32_t classID = 0;
    //! Human-readable description for the help.
    xoString description;
    //! Component's serial number. This field commonly used in devices, contains unique number associated with a certain device.
    xoUInt32 serialNumber;
    //! Major version of the component.
    unsigned char &version;
    //! Minor version of the component.
    unsigned char &versionMinor;
    //! This field can be filled with arbitrary information about component release (e.g. build date and time, release number etc.).
    xoString releaseInfo;
    //! This field commonly used in devices. Can contain CPU description or something else about hardware.
    xoString hardwareInfo;
    //! Historical thing from devices. Indicates how many times the device burned out. ]:->
    xoUInt32 burnCount;

    //! @brief Set an icon to the component.
    //! @details Component icon is used in the UI.
    //! @param icon The image containing icon.
    void setIcon(QImage icon);

    //! @brief Set an icon to the component from file.
    //! @details This is an overloaded method.
    //! @param filename Name of icon file. Supported formats matches ones in Qt's QImage.
    void setIcon(QString filename);

    //! @brief Create an input of the component.
    //! @details Creates a volatile writable object bound to a given variable.
    //! Also meta-values can be associated with the object.
    //! @param name Name of the object.
    //! @param var Reference to variable to bind.
    //! @return reference to created object.
    template <class T>
    xoObject<T> &createInput(QString name, xoObject<T> &obj)
    {
        obj.m_description.name = name;
        obj.m_description.flags |= ObjectBase::Input;
        bindObject(obj);
        return obj;
    }

    //! @brief Create an ouput of the component.
    //! @details Creates a volatile readable object bound to a given variable.
    //! Also meta-values can be associated with the object.
    //! @param name Name of the object.
    //! @param var Reference to variable to bind.
    //! @return reference to created object.
    template <class T>
    xoObject<T> &createOutput(QString name, xoObject<T> &obj)
    {
        obj.m_description.name = name;
        obj.m_description.flags |= ObjectBase::Output;
        bindObject(obj);
        return obj;
    }

    //! @brief Create a setting of the component.
    //! @details Creates a non-volatile read-write object bound to a given variable.
    //! Also meta-values can be associated with the object.
    //! @param name Name of the object.
    //! @param var Reference to variable to bind.
    //! @return reference to created object.
    template <class T>
    xoObject<T> &createSetting(QString name, xoObject<T> &obj)
    {
        obj.m_description.name = name;
        obj.m_description.flags |= ObjectBase::Setting;
        bindObject(obj);
        return obj;
    }

    //! @brief Get the object by name.
    //! @details If the object with given name is created,
    //! the method return a constant pointer to the object base.
    //! You can cast it manually to const ONBObject<T>* where T must match the type of bound variable.
    //! @param name The object name.
    //! @return A constant pointer to the object. If an object with given name does not exist, nullptr is returned.
    const ObjectBase *object(QString name) const;

    //! @brief Rebind the existing object to another variable.
    //! @details Use this method if the bound variable is moved or reallocated.
    //! This method also can be used for changing variable type at run-time.
    //! @param name The name of the object.
    //! @param var Reference to variable to bind.
    //! @return true if rebind was successful, false if object does not exist.
    template <class T>
    bool rebindObject(QString name, xoObject<T> &obj)
    {
        xoObject<T> *oldObj = dynamic_cast<xoObject<T>*>(m_objectMap.value(name, nullptr));
        if (oldObj)
        {
            int oid = oldObj->description().id;
            obj.copyInfoFrom(*oldObj);
            obj.m_component = this;
            m_objects[oid] = &static_cast<ObjectBase&>(obj);
            m_objectMap[name] = &static_cast<ObjectBase&>(obj);
            sendObjectInfo(oid);
            return true;
        }
        return false;
    }

    //! @brief Rename existing object.
    //! @details Use this method to rename the existing object at run-time.
    //! The new name must not match the existing objects' names.
    //! @param oldName The name of existing object.
    //! @param newName The desired name.
    //! @return true if success, false if the object with given oldName does not exist.
    bool renameObject(QString oldName, QString newName);

    //! @brief Delete existing object.
    //! @details Use this method to remove the object from component at run-time.
    //! @param name The name of existing object.
    //! @return true if success, false if the object with given oldName does not exist.
    bool deleteObject(QString name);

    //! @brief Send the object to the core immediately.
    //! @details If the object with given name does not exist the method does nothing.
    //! @param name The name of the object.
    void sendObject(QString name);

    //! @brief Component creation event.
    //! @details It is called when component is created by core.
    //! Reimplement this method to initialize your component logic, e.g. run timers, threads, allocate buffers etc.
    virtual void onCreate() {}

    //! @brief Component deletion event.
    //! @details It is called when component is deleted by core.
    //! Reimplement this method to terminate your component logic, e.g. stop timers, threads, deallocate buffers etc.
    virtual void onDestroy() {}

    //! @brief Event of object request.
    //! @details It is called when the core requests the object with the given name
    //! or the sampling interval has come.
    //! The event is called before sending object to the core.
    //! You should complete the bound variable with actual value when the event is arrived.
    //! @param name The name of the requested object.
    virtual void objectRequestEvent(QString name) {(void)name;}

    //! @brief Event of object receive.
    //! @details It is called when the core sends the object with the given name
    //! after its value is written to the bound variable.
    //! So you can use the variable with its actual value just inside this method.
    //! @param name The name of the requested object.
    virtual void objectReceiveEvent(QString name) {(void)name;}

    //! @brief Event of object change.
    //! @details This method is similar to objectReceiveEvent() but
    //! it is called when the value of the variable was changed only.
    //! @param name The name of the requested object.
    virtual void objectChangeEvent(QString name) {emit objectChanged(name);}

    //! @brief Send the arbitrary message to the core's log output.
    void log(QString message);

private:
    //! ComponentID of the component
    unsigned short m_id = 0;
    //! The name of the component. Used to name the type, must be unique.
    xoString m_componentName;
    //! ClassID of the component. Class identifier, must be unique. There is classification table (must be somewhere).
    xoUInt32 m_classID;
    //! Object dictionary
    QList<ObjectBase*> m_objects;
    //! Object access by name
    QMap<QString, ObjectBase*> m_objectMap;
    //! Service object dictionary
    QList<ObjectBase*> m_svcObjects;
    //! duplicates m_objects.count() because methods invokation isn't supported yet
    xoUInt8 m_objectCount; //! @todo: make the possible number of objects more than 256, everywhere...

    //! map of timers for object auto-sending, accessed by ObjectID
    QMap<unsigned char, QTimer*> m_autoSendTimers;

    //! create binding for service object
    unsigned char bindSvcObject(QString name, ObjectBase &obj, ObjectBase::Flags flags);

    //! @brief Make the object ready to send.
    //! @details The object will be sent as soon as its next sampling interval has come.
    //! The object is sent immediately if the sampling interval is set to special value of zero.
    //! If the object with given name does not exist the method does nothing.
    //! @param name The name of the object.
    //! @param notifyAnyway Maybe it's better to use sendObject()?
    void touchOutput(QString name);//, bool notifyAnyway = false);

    friend class ObjectBase; // DIRTY HACK

    void parseServiceMessage(unsigned char oid, const QByteArray &data);
    void parseGlobalServiceMessage(unsigned char aid);
    void parseMessage(unsigned char oid, const QByteArray &data);
    void sendServiceMessage(unsigned char oid, const QByteArray &data = QByteArray());
    void sendMessage(unsigned char oid, const QByteArray &data);

    void sendSvcObject(unsigned char oid);
    void sendObjectInfo(unsigned char oid);

    unsigned char bindObject(ObjectBase &obj);
    void setId(unsigned short id) {m_id = id;}

    bool m_created = false;
    xoString m_instanceName;
    xoUInt16 m_version;
    xoUInt8 m_busType;
    xoByteArray m_iconData;

    friend class ModuleBaseONB;

    QElapsedTimer *m_timestampTimer = nullptr;

private slots:
    void receiveData(const ONBPacket &packet);

signals:
    //! @private
    void newData(const ONBPacket &packet);
};



#endif // COMPONENTBASE_H
