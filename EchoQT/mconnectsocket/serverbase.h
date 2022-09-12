#ifndef SERVERBASE_H
#define SERVERBASE_H
#include<QTcpServer>
#include<QVector>
#include<mconnectsokcet.h>
class ServerBase : public QTcpServer
{
    Q_OBJECT
public:
    explicit ServerBase(QObject *parent = 0);
    ~ServerBase();
    void run();

private slots:
    void slotNewConnection();
    void slotRemoveClient();

private:
    QTcpServer* mTcpServer;
    QVector <mConnectSocket*> Clients;
};

#endif // SERVERBASE_H
