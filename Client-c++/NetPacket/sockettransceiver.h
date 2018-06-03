#ifndef SOCKETTRANSCEIVER_H
#define SOCKETTRANSCEIVER_H
#if _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

#include <QObject>
#include <QTcpSocket>
#include <QThread>

class NetDAO;
class SocketTransceiver : public QObject
{
    Q_OBJECT
public:
    explicit SocketTransceiver(QObject *parent = 0);
    ~SocketTransceiver();

    bool startup();
    void stop();

signals:
    void socketReceived(const QByteArray &byteData);

public slots:
    void socketSend(const QByteArray &byteData);

private:
    QTcpSocket *m_pTcpSocket;
    NetDAO *m_pNetDao;

private slots:
    void slotSocketRead();
};

#endif // SOCKETTRANSCEIVER_H
