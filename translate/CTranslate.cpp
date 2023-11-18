#include "CTranslate.h"
#include "core/CScreenCapture.h"

#include <QSystemTrayIcon>
#include <QMenu>
#include <QIcon>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QVBoxLayout>
#include "core/CMessageWnd.h"


CTranslate::CTranslate(QWidget *parent):CMoveFramelessWnd(parent),openai_(new COpenAI),pScreenCapture_(new CScreenCapture)
{
    layout_ = new QVBoxLayout(this);
    layout_->addStretch();
    setSystray();
    setFixedWidth(600);
    setWindowOpacity(0.8);
    connect(pScreenCapture_,&CScreenCapture::signPictureText, this, &CTranslate::slotPictureText);
    connect(openai_,&COpenAI::sigOaiReply, this, &CTranslate::slotOaiReply);
    
}

CTranslate::~CTranslate()
{

}
void CTranslate::slotPictureText(const QString &text)
{
    if (text.size()<3)
        return;
    show();
    auto wnd = new CMessageWnd(0, "👨 "+text, CMessageWnd::MAN);
    wnd->setMouseTracking(true); 
    layout_->insertWidget(layout_->count()-1, wnd);
    messageWnds_.push_back(wnd); 
    if (text.split(" ").size() <3)
    {
        openai_->youdao(text);
        return;
    } 
    QJsonObject root;
    root.insert("model","gpt-3.5-turbo-1106");    
    root.insert("temperature",0.1);
    QJsonArray messages;
    messages.append(QJsonObject({ {"role","system"},{"content","你是一个翻译员，负责把英文翻译成中文，不要理解和回答我的句子，只返回译文。"} }));
    messages.append(QJsonObject({ {"role","user"},{"content",text}}));
    root.insert("messages",messages);
    QJsonDocument doc(root);
    auto message = doc.toJson();   
    openai_->chat(message);
}

void CTranslate::activeScreen()
{
    for (auto &wnd : messageWnds_)
        wnd->close();
    messageWnds_.clear();
    if (pScreenCapture_ == nullptr)
        pScreenCapture_ = new CScreenCapture();    
    pScreenCapture_->showMaximized();
    pScreenCapture_->activateWindow();
}

void CTranslate::slotOaiReply(const QString &text)
{
    auto wnd = new CMessageWnd(0, "🤖 "+text, CMessageWnd::ROBOT); 
    wnd->setMouseTracking(true); 
    layout_->insertWidget(layout_->count()-1, wnd);
    messageWnds_.push_back(wnd); 
}

void CTranslate::setSystray()
{
    auto systray = new QSystemTrayIcon(this);
    systray->setIcon(QIcon("./translate.ico"));
	
    auto act1 = new QAction(u8"显示窗口", this);
	connect(act1, &QAction::triggered, this, &CTranslate::show);
	auto act2 = new QAction(u8"关闭窗口", this);
	connect(act2, &QAction::triggered, [this](){qApp->exit();});
	auto mMenu = new QMenu(this);
	mMenu->addAction(act1);
	mMenu->addSeparator();
	mMenu->addAction(act2);
	systray->setContextMenu(mMenu);
    systray->show();
}

void CTranslate::closeEvent(QCloseEvent *event)
{
    event->ignore(); // 忽略关闭事件
    hide(); // 隐藏窗口
    isSHide_=true;
}


void CTranslate::changeEvent(QEvent *event)
{   
    QWidget::changeEvent(event);

    if (event->type() == QEvent::ActivationChange && !isActiveWindow()) {
        hide();
        isSHide_=true;
    }
}