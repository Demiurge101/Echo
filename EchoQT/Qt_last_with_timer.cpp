#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QTimer>

class ServerBase : public QTcpServer
{
public:
    explicit ServerBase(QObject *parent = 0);
    ~ServerBase();
    void run();

private slots:
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnected();

private:
    QTcpServer* mTcpServer = new QTcpServer(this);
    QTcpSocket* mTcpSocket = new QTcpSocket(this);
    QVector <QTcpSocket*> Clients;
    QVector <int> timers;
    bool flagTimeout = true;
    void timerEvent(QTimerEvent *event);
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
    delete mTcpSocket;
    for(int i = 0; i < Clients.size(); i++)
        delete Clients[i];
    Clients.clear();
    for(int i = 0; i < timers.size(); i++)
        killTimer(i);
    timers.clear();
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
    timers.push_back(startTimer(30000));
    qDebug() << "Client connect " << timers[Clients.indexOf(mTcpSocket)];
}

void ServerBase::slotServerRead(){
    mTcpSocket = (QTcpSocket*)sender();
    killTimer(timers[Clients.indexOf(mTcpSocket)]);
    timers[Clients.indexOf(mTcpSocket)] = startTimer(30000);
    QByteArray array = mTcpSocket->readAll();
    mTcpSocket->write(array);
}

void ServerBase::slotClientDisconnected(){
    mTcpSocket = (QTcpSocket*)sender();
    if(!flagTimeout){
        //qDebug() << "index clients  " << Clients.indexOf(mTcpSocket);
        delete Clients[Clients.indexOf(mTcpSocket)];
        Clients.removeAll(mTcpSocket);
        flagTimeout = true;
    }
    else{
        mTcpSocket->close();
        qDebug() << "Client disconnect " << timers[Clients.indexOf(mTcpSocket)];
     }

}

void ServerBase :: timerEvent(QTimerEvent *event)
{
    mTcpSocket = Clients[timers.indexOf(event->timerId())];
    if(mTcpSocket->state()){
        mTcpSocket->close();
        qDebug() << "Timeout disconnect. Client " << event->timerId();
        killTimer(event->timerId());
        timers.removeAll(event->timerId());
        flagTimeout = false;
    }
    else{
        killTimer(event->timerId());
        timers.removeAll(event->timerId());
        delete Clients[Clients.indexOf(mTcpSocket)];
        Clients.removeAll(mTcpSocket);
    }

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ServerBase echo;
    echo.run();
    return a.exec();
}



