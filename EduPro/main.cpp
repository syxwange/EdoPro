

#include <QtWidgets/QApplication>
#include "CEduPro.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CEduPro* w =new CEduPro;   
    w->show();   
    return a.exec();
}
