#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    echo = new ServerBase(this);
}

MainWindow::~MainWindow()
{
    for(auto i : connections){
        i->close();
        newcon = i;
        delete newcon;
    }
    connections.clear();
    delete echo;
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    if(echo->state() && IsNotStarted){
        echo->run();
        IsNotStarted = false;
        ui->textBrowser->append("Server is started! ");

        connect(echo,&ServerBase::connectClient,this, &MainWindow::connected_Client,Qt::DirectConnection);
        connect(echo,&ServerBase::disc,this, &MainWindow::disconnect_Client,Qt::DirectConnection);

    }
    else if(!echo->state()){
        ui->textBrowser->append("Server is not started! ");
    }
    else{
        ui->textBrowser->append("Server is worked! ");
    }
}

void MainWindow::connected_Client()
{
    QString str;
    str = QString::number(echo->getlastDescriptor());
    ui->textBrowser->append("Connect client " + str);

    newcon = new windowConnect(echo->getlastDescriptor());
    newcon->show();

    connect(echo,&ServerBase::tMessage,newcon, &windowConnect::messageSlot, Qt::QueuedConnection);
    connect(newcon,&windowConnect::closeWindow,this, &MainWindow::Close_window,Qt::QueuedConnection);

    connections[echo->getlastDescriptor()] = newcon;
}

void MainWindow::disconnect_Client()
{
    QString str;
    str = QString::number(echo->getlastDescriptor());
    ui->textBrowser->append("Disconnect client " + str);

    newcon = connections[echo->getlastDescriptor()];
    if(newcon != nullptr){
        newcon->close();
        delete newcon;
    }
    connections.remove(echo->getlastDescriptor());
}

void MainWindow::Close_window(int descriptor)
{
    echo->delsocket(descriptor);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    for(auto i : connections){
        i->close();
    }
}



