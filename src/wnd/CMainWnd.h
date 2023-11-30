#pragma once

#include "components/CMoveFramelessWnd.h"
#include "../core/UMessage.h"

class CStatusWnd;
class CTitileWnd;
class CCenterWnd;
class CMainWnd : public  CMoveFramelessWnd
{
    Q_OBJECT
public:    
    CMainWnd(QWidget *parent = nullptr); 

private: 
   
    CTitileWnd * titleWnd_;
    CCenterWnd * centerWnd_;
    CStatusWnd * statusWnd_;

};