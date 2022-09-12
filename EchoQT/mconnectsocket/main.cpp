#include <QCoreApplication>
#include <serverbase.h>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ServerBase echo;
    echo.run();
    return a.exec();
}

