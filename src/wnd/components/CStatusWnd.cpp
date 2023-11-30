#include "CStatusWnd.h"
#include <QLabel>
#include <QHBoxLayout>

CStatusWnd::CStatusWnd(QWidget * parent):QFrame(parent)
{
    leftLabel_ = new QLabel(this);
    rightLabel_ = new QLabel(this);
    
    setLeftText("");
    setRightText("gpts");    
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(leftLabel_);
    layout->addStretch();
    layout->addWidget(rightLabel_);
    layout->setContentsMargins(10,0,10,0);

    setFixedHeight(30);
    setStyleSheet("background-color: #b5d6e7;");
}

void CStatusWnd::setLeftText(const QString &text)
{
    leftLabel_->setText(text);
}

void CStatusWnd::setRightText(const QString &text)
{
    rightLabel_->setText(text);
}

void CStatusWnd::setFont(const QFont &font)
{
    leftLabel_->setFont(font);
    rightLabel_->setFont(font);
}
