/**************************************************************
 * File Name   : network.cpp
 * Author      : ThreeDog
 * Date        : Sat Jun 02 16:46:41 2018
 * Description : 网络模块类，只负责网络信息收发处理留给主逻辑
 *
 **************************************************************/

#include "network.h"

NetWork::NetWork(QObject *parent)
    :QObject(parent)
{
    m_pTcpSocket = new QTcpSocket(this);
    connect(m_pTcpSocket,SIGNAL(readyRead()),this,SLOT(slot_socketRead()));
}

NetWork::~NetWork()
{

}

bool NetWork::startup()
{
    m_pTcpSocket->connectToHost(HOST,PORT);

    if(m_pTcpSocket->waitForConnected(3000)){
        return true;
    }else{
        return false;
    }
}

void NetWork::stop()
{
    m_pTcpSocket->disconnectFromHost();
}

void NetWork::slot_socketSend(const QJsonObject &jsonData)
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    QByteArray byteData = QJsonDocument(jsonData).toJson();

    if(m_pTcpSocket->isOpen()){
        //        m_pTcpSocket->write(QByteArray::number(byteData.size()),sizeof(qint64));
        m_pTcpSocket->write(byteData);
        m_pTcpSocket->flush();
    }
}

void NetWork::slot_socketRead()
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);

//    static qint64 size = 0;
    while(1){
        QByteArray data = m_pTcpSocket->readAll();
        qDebug()<<data;
        QJsonParseError json_error;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data,&json_error);
        if(jsonDoc.isNull())
        {
            qDebug()<<json_error.errorString();
            continue;
        }
        QJsonObject jsonObj = jsonDoc.object();
        emit sig_socketReceived(jsonObj);
        break;
        /*
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
            emit sig_socketReceived(dt);
        }
        */
    }
}
