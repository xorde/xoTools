#ifndef MODULEBASEAPPONB_H
#define MODULEBASEAPPONB_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include "network/WebSocket/WebSocketClient.h"
#include "ModuleBaseONB.h"
#include "xotools_global.h"

//! @brief ONB module base for external modules.
//! @details Use it if you want create an external module.
class XOTOOLSSHARED_EXPORT ModuleBaseAppONB : public ModuleBaseONB
{
    Q_OBJECT

public:
    //! @brief Declare the module with given name and host and port.
    //! @param in_id The name of the module.
    //! @param host The name or IP address of the host to connect.
    //! @param port The port to connect.
    explicit ModuleBaseAppONB(QString in_id, QString host, QString port, QObject *parent = nullptr);

    //! Close the connection on destroying.
    ~ModuleBaseAppONB() override;

    virtual bool isActive() override;
    virtual bool work() override;

    //! @brief Try connect to the host.
    //! @return true if success or connection is already established, false if connection is failed.
    bool tryConnect();

    //! Disconnect from the host.
    void closeConnect();
    QString getName();
    void setHost(QString value);
    void setPort(QString value);

    QString getPort(){ return m_port; }
    QString getHost(){ return m_host; }

private:
    QString m_id;
    QString m_port;
    QString m_host;   
    WebSocketClient* m_pClientSocket = nullptr;

//signals:
//    void disconnection();
signals:
    void error(QAbstractSocket::SocketError error);
    void connectionDone();

protected:
    //! @private
    virtual void sendPacket(const ONBPacket &packet) override;
    //! @private
    virtual bool start() override;

private slots:
    void onConnect();

protected slots:
    //! @private:
    //! call receiveData() with data when it is received from hub
    virtual void receiveData(const QByteArray &data);
};

#endif // MODULEBASEAPPONB_H
