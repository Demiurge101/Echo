#ifndef MCONNECTSOKCET_H
#define MCONNECTSOKCET_H
#include<QTcpSocket>
#include<QTimer>

class mConnectSocket : public QObject{
    Q_OBJECT
public:
    mConnectSocket(QTcpSocket* sock, QObject* parent = 0);
    ~mConnectSocket();
    int getsocketId(){ return socketId; }
private:
    QTcpSocket* socket;
    QTimer* timer;
    int socketId;
signals:
    void disconnect();
    void transMessage(QByteArray array, int descriptor);

private slots:
    void SlotReadyRead();
    void SlotTimeoutDisconnect();
};

#endif // MCONNECTSOKCET_H
