

#include <QtWidgets/QApplication>
#include "core/CGlobalShortCut.h"
#include "CEduPro.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CEduPro* w = CEduPro::app();
    
    // CGlobalShortCut shortcut {};
    // a.installNativeEventFilter(&shortcut);
    // QObject::connect(&shortcut, &CGlobalShortCut::signActiveWin, &w, &CWgGpt::activeWindow);
    // QObject::connect(&shortcut, &CGlobalShortCut::signActiveScreen, &w, &CEduPro::activeScreen);
    
    w->show();   
    return a.exec();
}
