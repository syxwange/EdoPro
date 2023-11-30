#include "CMainWnd.h"
#include <QApplication>
#include <QHBoxLayout>
#include "components/CTitileWnd.h"
#include "components/CStatusWnd.h"
#include "components/CCenterWnd.h"
#include "../CEduPro.h"

CMainWnd::CMainWnd(QWidget * parent):CMoveFramelessWnd(parent),
    statusWnd_(new CStatusWnd(this)),centerWnd_(new CCenterWnd(this)),titleWnd_(new CTitileWnd(this))
{
    setWindowIcon(QIcon("./images/chatGPT.png"));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(1,1,1,1);
    layout->setSpacing(0);  
    statusWnd_->setMouseTracking(true);
    centerWnd_->setMouseTracking(true);
    titleWnd_->setMouseTracking(true);
    layout->addWidget(titleWnd_);
    layout->addWidget(centerWnd_);  
    layout->addWidget(statusWnd_);
    layout->setStretch(1,1);

    resize(900,1000);
    connect(titleWnd_,&CTitileWnd::signCloseWnd,this,&CMainWnd::close);
    connect(titleWnd_,&CTitileWnd::signMinWnd,this,&CMainWnd::showMinimized);
    connect(CEduPro::app(),&CEduPro::signStatusText,statusWnd_,&CStatusWnd::setLeftText);  
    connect(CEduPro::app(),&CEduPro::signStatusRightText,statusWnd_,&CStatusWnd::setRightText);  

}

