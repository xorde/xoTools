#ifndef WEBSOCKETCLIENT1_H
#define WEBSOCKETCLIENT1_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include "xotools_global.h"

class XOTOOLSSHARED_EXPORT WebSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketClient(const QUrl &url);
    explicit WebSocketClient(quint16 in_port, QString in_host);

    bool isOpenConnection() const;

    void setPort(quint16 in_port);
    void setUrl(const QUrl &in_url);
    void setHost(QString in_host);
    bool openConnection();    
    void closeConnection();
    bool sendData(QString in_data);
    bool sendData(const QByteArray& in_data);
signals:
    void closed();
    void connected();
    void readData(QString in_data);
    void readData(QByteArray in_data);
    void error(QAbstractSocket::SocketError error);
protected slots:
    void onConnected();    
    void onTextMessageReceived(QString message);
    void onBynaryMessageReceived(QByteArray in_data);
    void onError(QAbstractSocket::SocketError);

protected:
    QUrl m_url;
    quint16 m_port;
    QString m_host;

    bool m_is_opened;
    QWebSocket m_webSocket;

    void connectObject();
};

#endif // WEBSOCKETCLIENT1_H
