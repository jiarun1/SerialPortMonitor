#include "Surface/mainwindow.h"

#include <QApplication>
#include "Setting.h"


int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);

    Settings.Init();

    MainWindow w;
    w.show();

    return a.exec();
}
