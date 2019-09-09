#ifndef MODULEBASEONB_H
#define MODULEBASEONB_H

#include "xotools_global.h"
#include "ComponentBase.h"
#include <QIcon>
#include <QJsonObject>
#include <QJsonArray>
#include <QElapsedTimer>
#include <QObject>
#include <QDebug>

//! @mainpage XORDE module development API
//!
//! \section intro Introduction
//! This document describes how to create the XORDE module,
//! declare your own components and make them functional.
//!
//! \section desc Description
//! XORDE consists of the core and modules. Module is the components pack
//! united in the separate application or shared library.\n
//! There are internal and external modules.
//! Internal modules implemented as shared libraries directly connected to the core (at least as it is planned).
//! The external ones connects via WebSocket from their own applications.
//! They can be local or remote (running on the another machine).
//! To create your own external module you should derive from ModuleBaseAppONB class.
//! To make the module visible to the core...
//! @warning I don't know what to do :c try connect maybe...
//!
//! All modules have the list of components.
//! Each component is the functional unit of the system.
//! It has meta-description and set of ONB objects.
//!
//! ONB object is the fundamental unit of ONB protocol
//! and is intended to exchange data between components.
//! Object is bound to component's variable and contains
//! its name, type description and meta-values associated with the variable.
//!
//! To create component derive from ComponentBase, then write description
//! in the constructor, create objects and bind it to the member variables.
//! At last use ModuleBaseONB::declareComponent() in the module constructor to make
//! the component visible to the core.
//!
//! @see ModuleBaseONB, ModuleBaseAppONB, ComponentBase, ONBObject

//! @brief The base class for XORDE module.
//!
//! XORDE module has an unique name, icon and contains a list of ONB components.
//!
//! These components are self-describing by means of ONB protocol.
//! They have its own name, icon, version info etc. and list of the Objects.
//!
//! Object is bound to a certain variable, and its value is changed by accessing the variable.
//! Data exchange is performed by implicit or explicit transferring the objects.
//!
//! The ModuleBaseONB class contains methods to declare and manage components.\n
//! If you want to create an external module, use ModuleBaseAppONB.
//! @see ModuleBaseAppONB, ComponentBase, ONBObject
class XOTOOLSSHARED_EXPORT ModuleBaseONB : public QObject
{
    Q_OBJECT

public:
    //! Default constructor. Runs timestamp timer.
    ModuleBaseONB(QObject* parent = nullptr);

    //! It had to destroy the components but something went wrong.
    ~ModuleBaseONB();

    //! @brief Declare the ONB component.
    //! @details The component must be derived from ComponentBase.
    //! @code
    //!  declareComponent(new MyONBComponent);
    //! @endcode
    //! @param component The component to declare.
    template<class T> void declareComponent(T *component)
    {
        ComponentBase *comp = static_cast<ComponentBase*>(component);
        m_classes << comp->classID;
        m_classInfo[comp->classID] = comp;
        m_creators[comp->classID] = [=](QString name)
        {
            ComponentBase *c = new T;
            if (!name.isEmpty())
                c->m_instanceName = name;
            return c;
        };
    }

    QByteArray getIcon();

    //! @brief Set an icon to the module.
    //! @details Component icon is used in the UI.
    //! @param icon The image containing icon.
    void setIcon(QImage icon);

    //! @brief Set an icon to the module from file.
    //! @details This is an overloaded method.
    //! @param filename Name of icon file. Supported formats matches ones in Qt's QImage.
    void setIcon(QString filename);

    //! @brief Get the component by id.
    //! @param id componentID assigned by the core.
    //! @return Pointer to the component. Returns false if the component with the given componentID does not exist
    ComponentBase *component(unsigned short id) {return m_componentsMap.value(id, nullptr);}

    //! @brief Send the arbitrary message to the core's log output.
    void log(QString message);

    //! WHY?? it must be in proxy!
    QList<QJsonObject> getClassesInfo() const;
    //! it's too.
    QJsonArray getComponentsConfigList() const;

    virtual bool start() = 0;
    virtual bool isActive() = 0;
    virtual bool work() = 0;

//    void connectToComponents();
//    void disconnectFromComponents();

    //! why?? why it is located here?
    virtual QJsonObject getJsonConfig() const;

private:
    QList<ComponentBase*> m_components;
    QMap<unsigned short, ComponentBase*> m_componentsMap;
    QList<uint32_t> m_classes;
    QHash<uint32_t, ComponentBase*> m_classInfo;
    QHash<uint32_t, std::function<ComponentBase*(QString)> > m_creators;

    QByteArray m_iconData;
    QElapsedTimer m_timestampTimer;

    bool isComponentExists(QString name);

    void parsePacket(const ONBPacket &packet);

//    void sendModuleID();
    void sendFail(const ONBPacket &packet);
    void sendKillComponent(unsigned short compID);    
    void sendClassList();

    // maybe make these protected?
    bool newComponent(uint32_t classID, QString name);
    bool deleteComponent(unsigned short compID);

signals:
    //! @private
    void componentInitialized(QJsonObject config);

    //! @brief Notify when the component is created with assigned componentID by the core.
    //! @param id Assigned componentID of the created component.
    void componentCreated(unsigned short id);

private slots:
    void sendClassInfoPacket(const ONBPacket &packet);
protected slots:
    //! @private
    //! Retranslate packet from component
    virtual void sendPacket(const ONBPacket &packet) = 0;

public slots:
    //! @private
    //! call receivePacket() with ONBPacket when it is received from hub
    void receivePacket(const ONBPacket &packet);
};

#endif // MODULEBASE_H
