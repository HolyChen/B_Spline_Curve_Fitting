#include "view/CentralWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CentralWidget w;
    w.show();
    return a.exec();
}
