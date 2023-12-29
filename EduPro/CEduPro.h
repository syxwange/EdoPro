#ifndef CEDUPRO_H
#define CEDUPRO_H

#pragma once
#include <QObject>
#include <vector>
#include "core/UMessage.h"

class CMainWnd;
class CScreenCapture;
class CGpts;


class CEduPro:public QObject
{
    Q_OBJECT
public:    

    ~CEduPro();
    CEduPro();
    
    void show();
private:  
    void slotGetGptWndData();    
    
      
    CMainWnd * win_;
    CGpts * gpts_;        
};

#endif