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
#include "../CMainWnd.h"
#include "CMultiGptWnd.h"
//#7ab5d4   f8fcfe
CCenterWnd::CCenterWnd(QWidget * parent):QWidget (parent)
{
    createMenuWnd();
    // createGptWnd();  

    // 
    hLayout_ = new QHBoxLayout(this);
    hLayout_->setContentsMargins(0,0,0,0);  
    hLayout_->addWidget(leftMenuWnd_);
    // hLayout_->addWidget(rightWnd_);    
    hLayout_->setSpacing(0); 


}



void CCenterWnd::createMenuWnd()
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
    connect(gpt,&CImgButton::clicked,[this](){emit CMainWnd::getInstance()->signCreateWnd ("gpts");});
    connect(english,&CImgButton::clicked,[this](){emit CMainWnd::getInstance()->signCreateWnd ("english");});
    connect(video,&CImgButton::clicked,[this](){emit CMainWnd::getInstance()->signCreateWnd ("video");});
    connect(task,&CImgButton::clicked,[this](){emit CMainWnd::getInstance()->signCreateWnd ("task");});
    connect(setting,&CImgButton::clicked,[this](){emit CMainWnd::getInstance()->signCreateWnd ("setting");});
    connect(user,&CImgButton::clicked,[this](){emit CMainWnd::getInstance()->signCreateWnd ("user");menuBtnClicked();});
    

    connect(gpt,&CImgButton::clicked,this,&CCenterWnd::menuBtnClicked);
    connect(english,&CImgButton::clicked,this,&CCenterWnd::menuBtnClicked);
    connect(video,&CImgButton::clicked,this,&CCenterWnd::menuBtnClicked);
    connect(task,&CImgButton::clicked,this,&CCenterWnd::menuBtnClicked);
    connect(setting,&CImgButton::clicked,this,&CCenterWnd::menuBtnClicked);
    connect(user,&CImgButton::clicked,this,&CCenterWnd::menuBtnClicked);  


}




void CCenterWnd::slotCreateWnd(const QString &name, QStringList datas)
{
    if (name=="gpts")
    {
        auto gptWnd = new CGptWnd(this);   
        connect(gptWnd,&CGptWnd::signAddRole,CMainWnd::getInstance(),&CMainWnd::signAddRole); 
        connect(gptWnd,&CGptWnd::signAskGpt,CMainWnd::getInstance(),&CMainWnd::signAskGpt);
        connect(CMainWnd::getInstance(),&CMainWnd::signOaiReply,gptWnd,&CGptWnd::slotOaiReply);   
        hLayout_->addWidget(gptWnd); 
        if (rightWnd_)
        {
            hLayout_->removeWidget(rightWnd_);       
            rightWnd_->deleteLater(); 
        }
          
        rightWnd_ = gptWnd;    
        gptWnd->slotReset(datas);
    }
    else if (name=="video")
    {   
        auto videoWnd = new CMultiGptWnd(this);
        connect(videoWnd,&CMultiGptWnd::signAddRole,CMainWnd::getInstance(),&CMainWnd::signAddRole); 
        connect(videoWnd,&CMultiGptWnd::signMultiAskGpt,CMainWnd::getInstance(),&CMainWnd::signMultiAskGpt);  
        connect(videoWnd,&CMultiGptWnd::signGetRoleInfo,CMainWnd::getInstance(),&CMainWnd::signGetRoleInfo);
        connect(videoWnd,&CMultiGptWnd::signDelRole,CMainWnd::getInstance(),&CMainWnd::signDelRole);
        connect(CMainWnd::getInstance(),&CMainWnd::signMultiOaiReply,videoWnd,&CMultiGptWnd::slotMultiOaiReply);
        videoWnd->slotReset(datas); 
        hLayout_->addWidget(videoWnd);
        hLayout_->removeWidget(rightWnd_);       
        rightWnd_->deleteLater(); 
        rightWnd_ = videoWnd;
    }
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


