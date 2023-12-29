#include "CMainWnd.h"
#include <Windows.h>
#include <dwmapi.h>
#include <QStatusBar>
#include <QLabel>
#include "components/CustomButton.h"
#include <QVBoxLayout>
#include "components/CGptWnd.h"
#include "../core/CGpts.h"


#pragma comment(lib, "dwmapi.lib")


CMainWnd::CMainWnd(QWidget *parent):QMainWindow(parent)
{

    initUI();  
    createMenu();

}

void CMainWnd::initUI()
{
    setWindowIcon(QIcon("./images/chatGPT.png"));
    setWindowTitle("EduPulse");
    COLORREF color = RGB(237, 247, 252);// 使用RGB宏创建COLORREF
    ::DwmSetWindowAttribute((HWND)this->winId(),DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR,&color,sizeof(COLORREF));
    auto stateBar = statusBar();   
    stateBar->setStyleSheet("QStatusBar { background-color: #b5d6e7; min-height: 30px; }");
    QLabel *rightLabel = new QLabel();
    stateBar->addPermanentWidget(rightLabel); // addPermanentWidget默认右对齐
    QWidget *centerWnd = new QWidget(this);
    auto layout = new QHBoxLayout(centerWnd); 
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    setCentralWidget(centerWnd);
    resize(900,1000);
    move(300, 150);
}

QWidget* CMainWnd::createMenu()
{
    auto menuWnd = new QWidget(this);
    menuWnd->setMouseTracking(true);
    menuWnd->setFixedWidth(50);
    menuWnd->setStyleSheet("background-color: #f8fcfe;");
    QVBoxLayout *layout = new QVBoxLayout(menuWnd);
    layout->setSpacing(20);
    auto gpt = new CImgButton("./images/gpt-black.png","./images/gpt-blue.png",QSize(30,30),menuWnd);
    auto setting = new CImgButton("./images/setting-black.png","./images/setting-blue.png",QSize(30,30),menuWnd);
    auto user = new CImgButton("./images/user-black.png","./images/user-blue.png",QSize(30,30),menuWnd);
    auto task = new CImgButton("./images/task-black.png","./images/task-blue.png",QSize(30,30),menuWnd);
    auto video = new CImgButton("./images/video-black.png","./images/video-blue.png",QSize(30,30),menuWnd);
    auto english = new CImgButton("./images/english-black.png","./images/english-blue.png",QSize(30,30),menuWnd);

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

    connect(gpt,&CImgButton::clicked,this,&CMainWnd::signCreateGptWnd);
    // connect(english,&CImgButton::clicked,this,&CCenterWnd::englishBtnClicked);
    // connect(video,&CImgButton::clicked,this,&CCenterWnd::videoBtnClicked);
    // connect(task,&CImgButton::clicked,this,&CCenterWnd::taskBtnClicked);

    centralWidget()->layout()->addWidget(menuWnd);  
    return menuWnd;
}

void CMainWnd::createGptWnd(CGpts* gpts)
{
    
    if(gptWnd_ != nullptr)  
        return; 
    gptWnd_ = new CGptWnd(gpts,this);    
    
    if (currentWnd_ ) {
        centralWidget()->layout()->removeWidget(currentWnd_);        
        currentWnd_->deleteLater();
    }  
    centralWidget()->layout()->addWidget(gptWnd_);   
    currentWnd_ = gptWnd_;  
}

void CMainWnd::slotStatusLeftText(const QString &text)
{
    statusBar()->showMessage(text);
}

void CMainWnd::slotStatusRightText(const QString &text)
{
    statusBar()->findChild<QLabel*>()->setText(text);
}

void CMainWnd::menuClick(CImgButton* sendBtn)
{ 
    for (const auto& btn : menuBtns_)
    {
        if (btn != sendBtn) 
            btn->slotSetUnClicked();
    }
}