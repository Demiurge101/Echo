#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>

class ServerBase : public QTcpServer
{
public:
    explicit ServerBase(QObject *parent = 0);
    ~ServerBase();
    void run();

public slots:
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnected();

private:
    QTcpServer* mTcpServer = new QTcpServer(this);
    QTcpSocket* mTcpSocket;
};

ServerBase::ServerBase(QObject *parent) : QTcpServer(parent){

    if(!mTcpServer->listen(QHostAddress::LocalHost, 23)){
        qDebug() << "server is not started";
    } else {
        qDebug() << "server is started";
    }
}

ServerBase::~ServerBase(){

    delete mTcpSocket;
    delete mTcpServer;
}

void ServerBase::run(){
    connect(mTcpServer, &QTcpServer::newConnection, this, &ServerBase::slotNewConnection, Qt::DirectConnection);
}

void ServerBase::slotNewConnection(){
    mTcpSocket = mTcpServer->nextPendingConnection();

    mTcpSocket->write("Welcome to Echo!\r\n");

    connect(mTcpSocket, &QTcpSocket::readyRead, this, &ServerBase::slotServerRead, Qt::QueuedConnection);
    connect(mTcpSocket, &QTcpSocket::disconnected, this, &ServerBase::slotClientDisconnected, Qt::QueuedConnection);

}

void ServerBase::slotServerRead(){

    while(mTcpSocket->bytesAvailable()>0)
    {
    QByteArray array = mTcpSocket->readAll();

    mTcpSocket->write(array);
    }
}

void ServerBase::slotClientDisconnected(){

    mTcpSocket->close();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ServerBase echo;
    echo.run();
    return a.exec();
}



