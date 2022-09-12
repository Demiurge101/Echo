#include "mconnectsokcet.h"

mConnectSocket::mConnectSocket(QTcpSocket *sock, QObject *parent){
    socket = sock;
    socketId = sock->socketDescriptor();
    socket->write("Welcome to Echo!\r\n");

    timer = new QTimer(this);
    timer->start(30000);

    connect(timer,&QTimer::timeout, this, &mConnectSocket::SlotTimeoutDisconnect, Qt::DirectConnection);
    connect(socket, &QTcpSocket::readyRead, this, &mConnectSocket::SlotReadyRead, Qt::DirectConnection);
    connect(socket, &QTcpSocket::disconnected, this, &mConnectSocket::disconnect, Qt::DirectConnection);
}

mConnectSocket::~mConnectSocket(){
    if(timer->isActive()){
        socket->close();
        timer->stop();
        qDebug() << "Client disconnect " << socketId;
    }
    socketId = 0;
    delete socket;
    delete timer;
}

void mConnectSocket::SlotReadyRead(){
    timer->stop();
    timer->start(30000);
    QByteArray array = socket->readAll();
    socket->write(array);
}

void mConnectSocket::SlotTimeoutDisconnect()
{
    socket->close();
    timer->stop();
    qDebug() << "Timeout disconnect " << socketId;
}
