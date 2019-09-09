#include "ModuleBaseAppONB.h"

#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QFile>

ModuleBaseAppONB::ModuleBaseAppONB(QString in_id, QString host, QString port, QObject *parent) : ModuleBaseONB(parent),
  m_id(in_id),
  m_host(host),
  m_port(port)
{
    QCommandLineOption ipOption(QStringList() << "i" << "ip",
                QCoreApplication::translate("Generic", "Server ip-adress."),
                QCoreApplication::translate("Generic", "Server ip"));

    QCommandLineOption portOption(QStringList() << "p" << "port",
                QCoreApplication::translate("Generic", "Server port."),
                QCoreApplication::translate("Generic", "Server port"));

    QCommandLineParser parser;
    parser.addOption(ipOption);
    parser.addOption(portOption);

    //ничего не передано в конструктор
    if(m_host.isEmpty() || m_port.isEmpty())
    {
        parser.process(QCoreApplication::instance()->arguments());

        m_host = parser.value(ipOption);
        m_port = parser.value(portOption);
    }

    //нет параметров командной строки
    if(m_host.isEmpty() || m_port.isEmpty())
    {

        QFile file("connection.ini");

        if(file.open(QIODevice::ReadOnly))
        {
            QString contents = file.readAll();
            QStringList args;
            args << "";
            args << contents.split(" ");
            parser.process(args);
        }
    }

    //ничего не найдено вообще
    if(m_host.isEmpty() || m_port.isEmpty())
    {
        m_host = "127.0.0.1";
        m_port = "8080";
    }
}

ModuleBaseAppONB::~ModuleBaseAppONB()
{
    if (m_pClientSocket)
    {
        delete m_pClientSocket;
        m_pClientSocket = nullptr;
    }
}

bool ModuleBaseAppONB::work()
{
    return true;
}

bool ModuleBaseAppONB::isActive()
{
    if (!m_pClientSocket)
        return false;

    return m_pClientSocket->isOpenConnection();
}

bool ModuleBaseAppONB::start()
{
    quint16 port = static_cast<quint16>(m_port.toInt());
    m_pClientSocket = new WebSocketClient(port, m_host);

//    connect(m_pClientSocket, SIGNAL(closed()), this, SIGNAL(disconnection()));
    connect(m_pClientSocket, SIGNAL(connected()), this, SLOT(onConnect()));
//    connect(m_pClientSocket, SIGNAL(readData(QString)), this, SLOT(slotReadData(QString)));
    connect(m_pClientSocket, SIGNAL(readData(QByteArray)), this, SLOT(receiveData(QByteArray)));
    connect(m_pClientSocket, &WebSocketClient::error, [=](QAbstractSocket::SocketError e){ QTimer::singleShot(5000, this, &ModuleBaseAppONB::start);  emit error(e); });

    return m_pClientSocket->openConnection();
}

void ModuleBaseAppONB::onConnect()
{
    m_pClientSocket->sendData(m_id);
    emit connectionDone();
}

void ModuleBaseAppONB::receiveData(const QByteArray &data)
{
    receivePacket(ONBPacket(data));
}

bool ModuleBaseAppONB::tryConnect()
{
    if (!isActive())
        start();

    return true;
}

void ModuleBaseAppONB::closeConnect()
{
    m_pClientSocket->closeConnection();
}

QString ModuleBaseAppONB::getName()
{
    return m_id;
}

void ModuleBaseAppONB::setHost(QString value)
{
    m_host = value;
}

void ModuleBaseAppONB::setPort(QString value)
{
    m_port = value;
}

void ModuleBaseAppONB::sendPacket(const ONBPacket &packet)
{
    QByteArray data;
    packet.writePacket(data);
    m_pClientSocket->sendData(data);
}

