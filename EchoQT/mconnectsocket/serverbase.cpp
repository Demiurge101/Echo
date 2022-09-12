#include "serverbase.h"

ServerBase::ServerBase(QObject *parent) : QTcpServer(parent){

    mTcpServer = new QTcpServer(this);

    if(!mTcpServer->listen(QHostAddress :: LocalHost, 23)){
        qDebug() << "server is not started";
    } else {
        qDebug() << "server is started";
    }
}

ServerBase::~ServerBase(){
    delete mTcpServer;
    for(int i = 0; i < Clients.size(); i++)
        delete Clients[i];
    Clients.clear();

}

void ServerBase::run(){
    connect(mTcpServer, &QTcpServer::newConnection, this, &ServerBase::slotNewConnection, Qt::DirectConnection);
}

void ServerBase::slotNewConnection(){

    QTcpSocket* mTcpSocket = mTcpServer->nextPendingConnection();
    mConnectSocket* mSocket = new mConnectSocket(mTcpSocket);

    connect(mSocket,&mConnectSocket::disconnect, this, &ServerBase::slotRemoveClient, Qt::QueuedConnection);

    Clients.push_back(mSocket);
    qDebug() << "Client connect " << mSocket->getsocketId();
}

void ServerBase::slotRemoveClient()
{
    mConnectSocket* mSocket = (mConnectSocket*) sender();
    delete mSocket;
    Clients.removeAll(mSocket);
}
