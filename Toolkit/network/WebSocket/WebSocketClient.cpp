#include "WebSocketClient.h"
#include <QtCore/QDebug>

QT_USE_NAMESPACE

WebSocketClient::WebSocketClient(const QUrl &url)
    : m_url(url), m_is_opened(false)
{
    connectObject();
}

WebSocketClient::WebSocketClient(quint16 in_port, QString in_host) : m_is_opened(false)
{
    m_host = in_host;
    m_port = in_port;
    QString port = QString::number(in_port);
    QString address = "ws://" + m_host + ":" + port;
    m_url = QUrl(address);

    connectObject();
}

void WebSocketClient::connectObject()
{
    connect(&m_webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::closed);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextMessageReceived);
    connect(&m_webSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketClient::onBynaryMessageReceived);
    connect(&m_webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, &WebSocketClient::onError);
}

void WebSocketClient::setPort(quint16 in_port)
{
    m_port = in_port;
}

void WebSocketClient::setUrl(const QUrl& in_url)
{
    m_url = in_url;
}

void WebSocketClient::setHost(QString in_host)
{
    m_host = in_host;
}

bool WebSocketClient::openConnection()
{
    if (m_url.isEmpty() || !m_url.isValid())
        return false;

    m_webSocket.open(m_url);
    return true;
}

bool WebSocketClient::isOpenConnection() const
{
    return m_is_opened;
}

void WebSocketClient::closeConnection()
{
    m_webSocket.close();
}

bool WebSocketClient::sendData(QString in_data)
{
    qint64 sz = m_webSocket.sendTextMessage(in_data);
    return sz == in_data.size();
}

bool WebSocketClient::sendData(const QByteArray& in_data)
{
    qint64 sz = m_webSocket.sendBinaryMessage(in_data);
    return sz = in_data.size();
}

void WebSocketClient::onConnected()
{
    m_is_opened = true;
    emit connected();
}

void WebSocketClient::onBynaryMessageReceived(QByteArray in_data)
{
    emit readData(in_data);
}

void WebSocketClient::onError(QAbstractSocket::SocketError e)
{
    emit error(e);
}

void WebSocketClient::onTextMessageReceived(QString message)
{    
    emit readData(message);
}
