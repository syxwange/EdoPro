#include "CMainWnd.h"
#include <QApplication>
#include <QHBoxLayout>
#include "components/CTitileWnd.h"
#include "components/CStatusWnd.h"
#include "components/CCenterWnd.h"

CMainWnd::CMainWnd(QWidget * parent):CMoveFramelessWnd(parent)
{
    setWindowIcon(QIcon("./images/chatGPT.png"));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(1,1,1,1);
    layout->setSpacing(0);
    titleWnd_ = new CTitileWnd(this);
    statusWnd_ = new CStatusWnd(this);
    centerWnd_ = new CCenterWnd(this);
    statusWnd_->setMouseTracking(true);
    centerWnd_->setMouseTracking(true);
    titleWnd_->setMouseTracking(true);
    layout->addWidget(titleWnd_);
    layout->addWidget(centerWnd_);  
    layout->addWidget(statusWnd_);
    layout->setStretch(1,1);

    // setStyleSheet("border-radius: 5;");
    resize(1100,1000);
    connect(dynamic_cast<CTitileWnd*>(titleWnd_),&CTitileWnd::signCloseWnd,this,&CMainWnd::close);
    connect(dynamic_cast<CTitileWnd*>(titleWnd_),&CTitileWnd::signMinWnd,this,&CMainWnd::showMinimized);
    connect(dynamic_cast<CCenterWnd*>(centerWnd_),&CCenterWnd::signAskGpt,this,&CMainWnd::signAskGpt);
    connect(this,&CMainWnd::signOaiReply,dynamic_cast<CCenterWnd*>(centerWnd_),&CCenterWnd::signOaiReply);
    }

    void CMainWnd::slotRolesName(const QStringList &roleNames)
    {
        dynamic_cast<CCenterWnd*>(centerWnd_)->slotRolesName(roleNames);
    }
