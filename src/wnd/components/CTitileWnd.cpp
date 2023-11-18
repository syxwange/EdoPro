#include "CTitileWnd.h"
#include <QIcon>
#include <QLabel>
#include <QHBoxLayout>
#include "CMessageWnd.h"
#include <QFrame>

CTitileWnd::CTitileWnd(QWidget *parent):QFrame(parent)
{
    // QIcon icon("./images/cahtGpt.png");
    imgLabel_ = new QLabel(this);
    titleLabel_ = new QLabel(this);

    setIcon("./images/chatGpt.png");
    setTitle("知识星球");
    auto closeBtn = new CImgButton("./images/close-black","./images/close-red");
    auto minBtn = new CImgButton("./images/min-black","./images/min-red");
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->setSpacing(20);
    hLayout->setContentsMargins(15,0,10,0);
    hLayout->addWidget(imgLabel_);
    hLayout->addWidget(titleLabel_);
    hLayout->addStretch();
    hLayout->addWidget(minBtn);
    hLayout->addWidget(closeBtn);
    setFixedHeight(40);    
    setStyleSheet("background-color: #edf7fc;");
    connect(closeBtn,&QPushButton::clicked,this,&CTitileWnd::signCloseWnd);
    connect(minBtn,&QPushButton::clicked,this,&CTitileWnd::signMinWnd);

}


void CTitileWnd::setTitle(const QString &title,const QFont& font)
{
    titleLabel_->setText(title);
    titleLabel_->setFont(font);
}

void CTitileWnd::setIcon(const QString &icon,const QSize &size)
{
    imgLabel_->setPixmap(QPixmap(icon));
    imgLabel_->setScaledContents(true);
    imgLabel_->setFixedSize(size);
}

