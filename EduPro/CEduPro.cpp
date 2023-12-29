#include "CEduPro.h"

#include "wnd/CMainWnd.h"
#include "core/CGpts.h"
#include "wnd/components/CGptWnd.h"


CEduPro::CEduPro():win_(new CMainWnd),gpts_(new CGpts)
{
    connect(win_,&CMainWnd::signCreateGptWnd,this,&CEduPro::slotGetGptWndData);   
    connect(gpts_,&CGpts::signStatusLeftText,win_,&CMainWnd::slotStatusLeftText);  
    connect(gpts_,&CGpts::signStatusRightText,win_,&CMainWnd::slotStatusRightText); 

}

CEduPro::~CEduPro()
{

}

void CEduPro::slotGetGptWndData()
{
    win_->createGptWnd(gpts_);
}

void CEduPro::show()
{ 
    win_->createGptWnd(gpts_);
    win_->show();    
}


