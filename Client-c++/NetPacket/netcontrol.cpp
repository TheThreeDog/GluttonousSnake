#if _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

#include "netcontrol.h"
#include "netdao.h"
#include <QMutex>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>

NetControl *NetControl::ms_object = NULL;

NetControl::NetControl(QObject *parent) : QObject(parent)
{
    m_bIsStart = false;

    m_pSocketTrans = new SocketTransceiver(this);

    connect(this,&NetControl::sig_sendNetData,m_pSocketTrans,&SocketTransceiver::socketSend);

    connect(m_pSocketTrans,&SocketTransceiver::socketReceived,this,&NetControl::slot_getNetData);

}

NetControl::~NetControl()
{
    m_socketThread.quit();
    m_socketThread.wait();
}

NetControl *NetControl::getInstance()
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    if(NULL == ms_object){
       ms_object = new NetControl;
    }
    return ms_object;
}

bool NetControl::startNet()
{
    if(m_bIsStart) return false;
    if(m_pSocketTrans->startup()){
        m_bIsStart = true;
    }else{
        m_bIsStart = false;
    }
    return m_bIsStart;
}

void NetControl::stopNet()
{
    m_bIsStart = false;
    m_pSocketTrans->stop();

}

bool NetControl::isStart()
{
    return m_bIsStart;
}

void NetControl::sendNetMsg(const QString &data)
{
//    QJsonDocument jsonDoc;
//    jsonDoc.setObject(jsonObj);
    QByteArray byteData = data.toLatin1();
    emit sig_sendNetData(byteData);
}


void NetControl::slot_getNetData(const QString &data)
{
//    QJsonDocument jsonDoc;
//    jsonDoc = QJsonDocument::fromJson(byteData);
    QByteArray byteData = data.toLatin1();
    emit sig_rcvedNetData(byteData);
}

void NetControl::resetNetWork()
{
    m_pSocketTrans->resetNetwork();
}
