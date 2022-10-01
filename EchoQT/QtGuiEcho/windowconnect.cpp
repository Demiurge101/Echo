#include "windowconnect.h"
#include "ui_windowconnect.h"

windowConnect::windowConnect(int descriptor, QWidget *parent) : QWidget(parent), ui(new Ui::windowConnect)
{
    ui->setupUi(this);
    windowid = descriptor;
    ui->label->setNum(windowid);
}

windowConnect::~windowConnect()
{
    windowid = 0;
    delete ui;
}

void windowConnect::messageSlot(QByteArray array, int descriptor)
{
    if(windowid == descriptor)
        ui->textBrowser->append(array);
}

void windowConnect::closeEvent(QCloseEvent *)
{
    if(this->isVisible())
        emit closeWindow(windowid);
}
