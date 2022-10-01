#ifndef WINDOWCONNECT_H
#define WINDOWCONNECT_H

#include <QWidget>

namespace Ui {
class windowConnect;
}

class windowConnect : public QWidget
{
    Q_OBJECT

public:
    explicit windowConnect(int descriptor, QWidget *parent = 0);
    ~windowConnect();

public slots:
    void messageSlot(QByteArray array, int descriptor);

signals:
    void closeWindow(int descriptor);

private:
    void closeEvent(QCloseEvent *);
    Ui::windowConnect *ui;
    int windowid;
};

#endif // WINDOWCONNECT_H
