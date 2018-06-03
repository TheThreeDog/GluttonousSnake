#ifndef NETDAO_H
#define NETDAO_H
#if _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif
#include <QObject>

class NetDAO
{
public:
    NetDAO();

    QString getServerAddress();
    QString getServerPort();

    void resetNetWork();
private:
    QString ip;
    int port;
//    void initNetConfigFile();


};

#endif // NETDAO_H
