/**************************************************************
 * File Name   : network.h
 * Author      : ThreeDog
 * Date        : Sat Jun 02 16:46:41 2018
 * Description : 网络模块类，只负责网络信息收发处理留给主逻辑
 *
 **************************************************************/
#ifndef _NETWORK_H_ 
#define _NETWORK_H_ 
#include <QTcpSocket>
#include <QObject>
#include <QMutexLocker>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#define HOST "127.0.0.1"
#define PORT 10001

class NetWork : public QObject
{
    Q_OBJECT
public:
    explicit NetWork(QObject * parent = 0);
    ~NetWork();
    bool startup();//启动网络
    void stop();//停止网络

signals:
    void sig_socketReceived(const QJsonObject jsonData);//收到数据发送信号，将数据内容通过信号发送

public slots:
    void slot_socketSend(const QJsonObject &jsonData);//发送数据的槽函数

private:
    QTcpSocket *m_pTcpSocket;

private slots:
    void slot_socketRead(); //接受数据的槽函数
};

#endif  //NETWORK
