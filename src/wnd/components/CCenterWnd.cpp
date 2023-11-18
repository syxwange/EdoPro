#include "CCenterWnd.h"
#include <QVBoxLayout>
#include "CMessageWnd.h"
#include <QHBoxLayout>
#include <QSplitter>
#include <QScrollArea>
#include <QTextEdit>
#include <QScrollBar>
#include <QDialog>
#include <QInputDialog>
#include "CGptWnd.h"
//#7ab5d4   f8fcfe
CCenterWnd::CCenterWnd(QWidget * parent):QWidget (parent)
{
    setMenuWnd();
    setGptWnd();  

    QHBoxLayout * hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(0,0,0,0);  
    hLayout->addWidget(leftMenuWnd_);
    hLayout->addWidget(rightWnd_);    
    hLayout->setSpacing(0); 

}

void CCenterWnd::slotRolesName(const QStringList roleNames)
{
    dynamic_cast<CGptWnd*>(rightWnd_)->slotRolesName(roleNames);
}

void CCenterWnd::setMenuWnd()
{
    leftMenuWnd_ = new QWidget(this);
    leftMenuWnd_->setMouseTracking(true);
    leftMenuWnd_->setFixedWidth(60);
    leftMenuWnd_->setStyleSheet("background-color: #f8fcfe;");
    QVBoxLayout *layout = new QVBoxLayout(leftMenuWnd_);
    layout->setSpacing(20);
    auto gpt = new CImgButton("./images/gpt-black.png","./images/gpt-blue.png",QSize(35,35),leftMenuWnd_);
    auto setting = new CImgButton("./images/setting-black.png","./images/setting-blue.png",QSize(35,35),leftMenuWnd_);
    auto user = new CImgButton("./images/user-black.png","./images/user-blue.png",QSize(35,35),leftMenuWnd_);
    auto task = new CImgButton("./images/task-black.png","./images/task-blue.png",QSize(35,35),leftMenuWnd_);
    auto video = new CImgButton("./images/video-black.png","./images/video-blue.png",QSize(35,35),leftMenuWnd_);
    auto english = new CImgButton("./images/english-black.png","./images/english-blue.png",QSize(35,35),leftMenuWnd_);
    gpt->setImg("./images/gpt-blue.png");
    layout->addWidget(gpt);
    layout->addWidget(english);
    layout->addWidget(video);
    layout->addWidget(task);
    layout->addStretch();  
    layout->addWidget(setting);
    layout->addWidget(user);

    menuBtns_.push_back(gpt);
    menuBtns_.push_back(english);
    menuBtns_.push_back(video);
    menuBtns_.push_back(task);
    menuBtns_.push_back(setting);
    menuBtns_.push_back(user);
    connect(gpt,&CImgButton::clicked,this,&CCenterWnd::setGptWnd);
    connect(english,&CImgButton::clicked,this,&CCenterWnd::setEnglishWnd);
    connect(video,&CImgButton::clicked,this,&CCenterWnd::setVideoWnd);
    connect(task,&CImgButton::clicked,this,&CCenterWnd::setTaskWnd);
    connect(setting,&CImgButton::clicked,this,&CCenterWnd::setSettingWnd);
    connect(user,&CImgButton::clicked,this,&CCenterWnd::setUserWnd);
   

    connect(gpt,&CImgButton::clicked,this,&CCenterWnd::menuBtnClicked);
    connect(english,&CImgButton::clicked,this,&CCenterWnd::menuBtnClicked);
    connect(video,&CImgButton::clicked,this,&CCenterWnd::menuBtnClicked);
    connect(task,&CImgButton::clicked,this,&CCenterWnd::menuBtnClicked);
    connect(setting,&CImgButton::clicked,this,&CCenterWnd::menuBtnClicked);
    connect(user,&CImgButton::clicked,this,&CCenterWnd::menuBtnClicked);  

}

void CCenterWnd::setGptWnd()
{    
    auto gptWnd = new CGptWnd(this);   
    connect(gptWnd,&CGptWnd::signAddGptRole,this,&CCenterWnd::signAddGptRole); 
    connect(gptWnd,&CGptWnd::signAskGpt,this,&CCenterWnd::signAskGpt);
    connect(this,&CCenterWnd::signOaiReply,gptWnd,&CGptWnd::slotOaiReply);
    rightWnd_ = gptWnd;
    
}

void CCenterWnd::setEnglishWnd()
{
}

void CCenterWnd::setVideoWnd()
{
}

void CCenterWnd::setTaskWnd()
{
}

void CCenterWnd::setSettingWnd()
{
}

void CCenterWnd::setUserWnd()
{
}


void CCenterWnd::menuBtnClicked()
{
    for (const auto& btn : menuBtns_)
    {
        auto senderObj = dynamic_cast<CImgButton*>(sender());
        if (btn != senderObj) 
            btn->slotSetUnClicked();   
    }
}


