#include "WebSocketServer.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>

QT_USE_NAMESPACE

WebSocketServer::WebSocketServer(quint16 in_port)
{
    m_port = in_port;
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Echo Server"), QWebSocketServer::NonSecureMode, this);
}

WebSocketServer::~WebSocketServer()
{
    closeAllConnection();
}

void WebSocketServer::setPort(quint16 in_port)
{
    m_port = in_port;
}

QList<QWebSocket*> WebSocketServer::getAllConnections()
{
    return m_clients;
}

bool WebSocketServer::isListening()
{
    return m_pWebSocketServer->isListening();
}

bool WebSocketServer::listen()
{
    if (0 == m_port)
        return false;

    if (m_pWebSocketServer->isListening())
        m_pWebSocketServer->close();

    if (m_pWebSocketServer->listen(QHostAddress::Any, m_port))
    {
        connect(m_pWebSocketServer, SIGNAL(closed()), this, SIGNAL(closed()));
        connect(m_pWebSocketServer, SIGNAL(newConnection()), this, SLOT(onConnected()));
        return true;
    }
    return false;
}

void WebSocketServer::closeAllConnection()
{
    m_pWebSocketServer->close();    
}

void WebSocketServer::sendDataAllClients(QString in_data)
{
    if (m_clients.isEmpty())
        return;

    for (QWebSocket* pSocket : m_clients)
        pSocket->sendTextMessage(in_data);
}

void WebSocketServer::sendDataAllClients(const QByteArray& in_data)
{
    if (m_clients.isEmpty())
        return;

    for (QWebSocket* pSocket : m_clients)
        pSocket->sendBinaryMessage(in_data);
}

bool WebSocketServer::sendData(QWebSocket* in_pConnection, QString in_data)
{
    qint64 sz = in_pConnection->sendTextMessage(in_data);
    return in_data.size() == sz;
}

bool WebSocketServer::sendData(QWebSocket* in_pConnection, const QByteArray& in_data)
{
    qint64 sz = in_pConnection->sendBinaryMessage(in_data);
    return in_data.size() == sz;
}

void WebSocketServer::onConnected()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);

    m_clients << pSocket;
    emit newConnection(pSocket);
}

void WebSocketServer::processTextMessage(QString message)
{
    QWebSocket* pConnection = qobject_cast<QWebSocket*>(sender());            
    emit readStringData(pConnection, message);
}

void WebSocketServer::processBinaryMessage(const QByteArray& message)
{
    QWebSocket* pConnection = qobject_cast<QWebSocket*>(sender());
    emit readBinaryData(pConnection, message);
}

void WebSocketServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket*>(sender());
    if (pClient)
    {
        m_clients.removeAll(pClient);
        emit clientDisconnect(pClient);
    }
}
