#if _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

#include "netdao.h"
#include <QMutex>
#include <QSettings>
#include <QFile>
#include <QHostAddress>
#include <QDebug>

#define NETCONFIGFILE "NetSettings.ini"
#define NET_GROUPNAME "NetGroup"

#define SERVERADDR "server_address"
#define SERVERPORT "server_port"

#define DEFAULT_SERVER_ADDR "127.0.0.1"
#define DEFAULT_SERVER_PORT 10001


NetDAO::NetDAO()
{
    ip = DEFAULT_SERVER_ADDR;
    port = DEFAULT_SERVER_PORT;
//    initNetConfigFile();
}


//void NetDAO::initNetConfigFile()
//{

//    static QMutex mutex;
//    QMutexLocker locker(&mutex);

//    if(QFile::exists(NETCONFIGFILE)) return;

//    QSettings sets(NETCONFIGFILE,QSettings::IniFormat);
//    sets.beginGroup(NET_GROUPNAME);
//    sets.setValue(SERVERADDR,DEFAULT_SERVER_ADDR);
//    sets.setValue(SERVERPORT,DEFAULT_SERVER_PORT);
//    sets.endGroup();
//}

QString NetDAO::getServerAddress()
{
//    QString sAddr;
//    QSettings sets(NETCONFIGFILE,QSettings::IniFormat);
//    sets.beginGroup(NET_GROUPNAME);
//    sAddr = sets.value(SERVERADDR).toString();
//    sets.endGroup();
//    return sAddr;
    return this->ip;
}

QString NetDAO::getServerPort()
{
//    QString sPort;
//    QSettings sets(NETCONFIGFILE,QSettings::IniFormat);
//    sets.beginGroup(NET_GROUPNAME);
//    sPort = sets.value(SERVERPORT).toString();
//    sets.endGroup();
//    return sPort;
    return QString("%1").arg(port);
}

void NetDAO::resetNetWork()
{
    ip = Setting::getInstance()->g_sIP;
    port = Setting::getInstance()->g_iPort;
}
