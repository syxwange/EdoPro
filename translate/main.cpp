

#include <QtWidgets/QApplication>
#include "core/CGlobalShortCut.h"
#include "CTranslate.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CTranslate w;
    
    CGlobalShortCut shortcut {};
    a.installNativeEventFilter(&shortcut);   
    QObject::connect(&shortcut, &CGlobalShortCut::signActiveScreen, &w, &CTranslate::activeScreen);
 
    return a.exec();
}
