#ifndef NETCONTROL_H
#define NETCONTROL_H
#if _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

#include <QObject>

#include "sockettransceiver.h"
#include "netdao.h"

#include <QThread>

class NetControl : public QObject
{
    Q_OBJECT
private:
    explicit NetControl(QObject *parent = 0);
    ~NetControl();
public:
    static NetControl *getInstance();

    //启动网络
    bool startNet();
    void stopNet();

    bool isStart();

    void sendNetMsg(const QString &data);

signals:
    void sig_sendNetData(const QByteArray &byteData);
    void sig_rcvedNetData(const QString &data);

public slots:
    void slot_getNetData(const QString &data);
    void resetNetWork();

private:
    static NetControl *ms_object;

    SocketTransceiver *m_pSocketTrans;

    bool m_bIsStart;

    QThread m_socketThread;

};

#endif // NETCONTROL_H
