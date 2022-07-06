#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>

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
    QVector <QTcpSocket*> Clients;
    int Counter = 0;
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

    Clients.push_back(mTcpSocket);

    connect(Clients.at(Counter), &QTcpSocket::readyRead, this, &ServerBase::slotServerRead, Qt::QueuedConnection);
    connect(Clients.at(Counter), &QTcpSocket::disconnected, this, &ServerBase::slotClientDisconnected, Qt::QueuedConnection);
    //Counter++;

}

void ServerBase::slotServerRead(){

    while(Clients[Counter]->bytesAvailable()>0)
    {
    QByteArray array = Clients[Counter]->readAll();

    Clients[Counter]->write(array);
    }
}

void ServerBase::slotClientDisconnected(){

    Clients[Counter]->close();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ServerBase echo;
    echo.run();
    return a.exec();
}
