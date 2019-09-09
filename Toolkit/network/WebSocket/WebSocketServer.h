#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)
#include "xotools_global.h"

class XOTOOLSSHARED_EXPORT WebSocketServer : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketServer(quint16 in_port);
    ~WebSocketServer();

    bool listen();
    void closeAllConnection();

    bool isListening();

    void sendDataAllClients(QString in_data);
    void sendDataAllClients(const QByteArray& in_data);

    bool sendData(QWebSocket* in_pConnection, QString in_data);
    bool sendData(QWebSocket* in_pConnection, const QByteArray& in_data);

    QList<QWebSocket*> getAllConnections();
    void setPort(quint16 in_port);

signals:
    void closed();
    void clientDisconnect(QWebSocket* in_pConnection);
    void newConnection(QWebSocket* in_pConnection);
    void readStringData(QWebSocket* in_pConnection, QString in_data);
    void readBinaryData(QWebSocket* in_pConnection, QByteArray in_data);
protected slots:
    void onConnected();
    void processTextMessage(QString message);
    void processBinaryMessage(const QByteArray &message);
    void socketDisconnected();

protected:
    quint16 m_port;
    QWebSocketServer* m_pWebSocketServer;
    QList<QWebSocket*> m_clients;
};
#endif // WEBSOCKETSERVER_H
