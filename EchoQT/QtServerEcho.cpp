#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>

class ServerBase : public QTcpServer
{
public:
    explicit ServerBase(QObject *parent = 0);
    ~ServerBase();
    void ShiftVector();
    void run();

public slots:
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnected();

private:
    QTcpServer* mTcpServer = new QTcpServer(this);
    QTcpSocket* mTcpSocket;
    QVector <QTcpSocket*> Clients;
};

ServerBase::ServerBase(QObject *parent) : QTcpServer(parent){

    if(!mTcpServer->listen(QHostAddress :: LocalHost, 23)){
        qDebug() << "server is not started";
    } else {
        qDebug() << "server is started";
    }
}

ServerBase::~ServerBase(){

    delete mTcpServer;
}

void ServerBase::ShiftVector()
{
    bool flagShift = true;
        while(flagShift){
            for(int i = 1; i < Clients.size(); i++){
               if(Clients[i - 1] == 0){
                   Clients[i - 1] = Clients[i];
                   for(int j = i + 1; j < Clients.size(); j++){
                       Clients[j - 1] = Clients[j];
                   }
                   Clients.erase(Clients.begin() + i);
                   flagShift = true;
                   continue;
               }
            }
            flagShift = false;
        }
}

void ServerBase::run(){
    connect(mTcpServer, &QTcpServer::newConnection, this, &ServerBase::slotNewConnection, Qt::DirectConnection);
}

void ServerBase::slotNewConnection(){


    mTcpSocket = mTcpServer->nextPendingConnection();

    mTcpSocket->write("Welcome to Echo!\r\n");

    connect(mTcpSocket, &QTcpSocket::readyRead, this, &ServerBase::slotServerRead, Qt::QueuedConnection);
    connect(mTcpSocket, &QTcpSocket::disconnected, this, &ServerBase::slotClientDisconnected, Qt::QueuedConnection);

    Clients.push_back(mTcpSocket);
}

void ServerBase::slotServerRead(){
    mTcpSocket = (QTcpSocket*)sender();

    for(int i = 0; i < Clients.size(); i++)
    {
        if(Clients.at(i) == mTcpSocket){
          QByteArray array = mTcpSocket->readAll();
           mTcpSocket->write(array);
        }

    }

}

void ServerBase::slotClientDisconnected(){
    mTcpSocket->close();
    for(int i = 0; i < Clients.size(); i++){
        if(Clients.at(i) == mTcpSocket){
            Clients[i] = 0;
            ShiftVector();
        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ServerBase echo;
    echo.run();
    return a.exec();
}



