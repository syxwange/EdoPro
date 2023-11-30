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
#include "../CMainWnd.h"
#include "CMultiGptWnd.h"
#include "../../CEduPro.h"
#include <ranges>
//#7ab5d4   f8fcfe




CCenterWnd::CCenterWnd(QWidget * parent):QWidget (parent)
{     
    hLayout_ = new QHBoxLayout(this);
    hLayout_->setContentsMargins(0,0,0,0); 
    hLayout_->setSpacing(0);  
    init();  
    hLayout_->addWidget(leftMenuWnd_);
    hLayout_->addWidget(rightWnd_);  
}



void CCenterWnd::init()
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

    connect(gpt,&CImgButton::clicked,this,&CCenterWnd::gptsBtnClicked);
    connect(english,&CImgButton::clicked,this,&CCenterWnd::englishBtnClicked);
    connect(video,&CImgButton::clicked,this,&CCenterWnd::videoBtnClicked);
    connect(task,&CImgButton::clicked,this,&CCenterWnd::taskBtnClicked);

    gpt->click();
}

void CCenterWnd::createGptWnd(const QString& name)
{
    CEduPro::app()->saveMsgAsJson();
    CEduPro::app()->messages_.clear();
    CEduPro::app()->currentTheme_=name;
    auto senderObj = dynamic_cast<CImgButton*>(sender());  
    for (auto btn:menuBtns_|std::views::filter([&senderObj](auto  i){return i!=senderObj;}))
        btn->slotSetUnClicked();
    auto videoWnd = new CMultiGptWnd(this); 
    videoWnd->initRoleBtn(name);
    connect(CEduPro::app(),&CEduPro::signMultiOaiReply,videoWnd,&CMultiGptWnd::slotMultiOaiReply);
    if (rightWnd_){
        hLayout_->removeWidget(rightWnd_);
        hLayout_->addWidget(videoWnd);
        rightWnd_->deleteLater();
    }
    rightWnd_ = videoWnd;
}

void CCenterWnd::gptsBtnClicked()
{
    auto senderObj = dynamic_cast<CImgButton*>(sender());
    btnClicked(senderObj);
    createGptWnd("gpts");
    emit CEduPro::app()->signStatusRightText("GPTs");

}

void CCenterWnd::videoBtnClicked()
{
    auto senderObj = dynamic_cast<CImgButton*>(sender());
    btnClicked(senderObj);
    createGptWnd("video");
    emit CEduPro::app()->signStatusRightText("video");
}

void CCenterWnd::englishBtnClicked()
{
    auto senderObj = dynamic_cast<CImgButton*>(sender());
    btnClicked(senderObj);
    createGptWnd("english");
    emit CEduPro::app()->signStatusRightText("english");
}

void CCenterWnd::taskBtnClicked()
{
    auto senderObj = dynamic_cast<CImgButton*>(sender());
    btnClicked(senderObj);
    createGptWnd("task");
    emit CEduPro::app()->signStatusRightText("task");
}

void CCenterWnd::btnClicked(CImgButton* sendBtn)
{ 
    for (const auto& btn : menuBtns_)
    {
        if (btn != sendBtn) 
            btn->slotSetUnClicked();
    }
}