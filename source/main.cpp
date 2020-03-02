#include "mainwindow.h"
#include "command.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(argc > 1)
    {
        Command cmd(argv[1], argv[2]);
        cmd.parsing();
    }
    else
    {
        MainWindow w;
        w.show();
        return a.exec();
    }
}
