#if _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

#include "sockettransceiver.h"
#include <QMutex>
#include <QDataStream>
#include "netdao.h"
#include <QDebug>
SocketTransceiver::SocketTransceiver(QObject *parent) : QObject(parent)
{
    m_pTcpSocket = new QTcpSocket(this);
    m_pNetDao = new NetDAO;

    connect(m_pTcpSocket,SIGNAL(readyRead()),this,SLOT(slotSocketRead()));

}

SocketTransceiver::~SocketTransceiver()
{
}

bool SocketTransceiver::startup()
{
    m_pTcpSocket->connectToHost(m_pNetDao->getServerAddress(),m_pNetDao->getServerPort().toInt());

    if(m_pTcpSocket->waitForConnected(3000)){
        return true;
    }else{
        return false;
    }
}

void SocketTransceiver::stop()
{
    m_pTcpSocket->disconnectFromHost();
}


void SocketTransceiver::socketSend(const QByteArray &byteData)
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    if(m_pTcpSocket->isOpen()){
//        m_pTcpSocket->write(QByteArray::number(byteData.size()),sizeof(qint64));
        m_pTcpSocket->write(byteData);
        m_pTcpSocket->flush();
    }
}


void SocketTransceiver::slotSocketRead()
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    static qint64 size = 0;
    while(1){
         if(size == 0){
            if(m_pTcpSocket->bytesAvailable() < sizeof(qint64))
                 return;
            size = m_pTcpSocket->read(sizeof(qint64)).toInt();
         }
         if(m_pTcpSocket->bytesAvailable() < size ){
            return;
         }else{
            QByteArray dt;
            dt = m_pTcpSocket->read(size);
            size = 0;
            emit socketReceived(dt);
         }
    }
}
