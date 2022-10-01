#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include "serverbase.h"
#include "windowconnect.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void connected_Client();
    void disconnect_Client();
    void Close_window(int descriptor);

private:
    void closeEvent(QCloseEvent *);
    Ui::MainWindow* ui;
    windowConnect* newcon;
    QMap<int,windowConnect*> connections;
    ServerBase* echo;
    bool IsNotStarted = true;
};
#endif // MAINWINDOW_H
