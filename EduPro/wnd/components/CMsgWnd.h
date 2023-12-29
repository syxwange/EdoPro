#pragma once

#include <QFrame>
#include <QLabel>
#include <QFont>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include "customButton.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QRadioButton>
#include <QCheckBox>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QImage>

class CMsgWnd:public QFrame
{
    Q_OBJECT

public:    

    CMsgWnd(size_t id,QString text, QVector<QString> rgbColor, QString role,QImage img={},QWidget *parent = nullptr): QFrame(parent),id_(id)
    {       
        
        setStyleSheet(QString("background-color:%1;border-radius: 10px;color:%2;").arg(rgbColor[0],rgbColor[1]));
        
        //设置文本label 设置字体大小，可换行
        auto textLabel_ = new QLabel(text, this);
        textLabel_->setTextInteractionFlags(textLabel_->textInteractionFlags() | Qt::TextSelectableByMouse);
        QFont font("微软雅黑", 13);
        textLabel_->setFont(font);        
        textLabel_->setWordWrap(true);

        //设置关闭图片按钮
        auto closeBtn = new CImgButton("./images/close-black.png","./images/close-red.png",QSize(16,16),this);
        
        QVBoxLayout *layout = new QVBoxLayout(this);   
        QHBoxLayout* titleLayout = new QHBoxLayout;
        // //设置头像label
        QLabel* nameLabel = new QLabel(role, this);  
        QFont fontName("微软雅黑", 11);
        nameLabel->setStyleSheet(QString("QLabel { color : %1; }").arg(rgbColor[2]));
        fontName.setBold(true);        
        nameLabel->setFont(fontName);  
        titleLayout->addWidget(nameLabel); 
        isSave_ = new QCheckBox("保存", this);
        titleLayout->addStretch();      
        titleLayout->addWidget(isSave_);
        titleLayout->addWidget(closeBtn);
        layout->addLayout(titleLayout);
        if (!img.isNull())
        {
            auto imgLabel = new QLabel(this);
            imgLabel->setPixmap(QPixmap::fromImage(img));
            // imgLabel->setFixedSize(100,100);
            imgLabel->setScaledContents(true);
            layout->addWidget(imgLabel);
        }
        layout->addWidget(textLabel_);           

        setLayout(layout);
  
        connect(isSave_, &QCheckBox::clicked,[this](){emit signSave(id_,isSave_->isChecked());});    
        connect(closeBtn, &QPushButton::clicked, this, &CMsgWnd::close);
        connect(closeBtn, &QPushButton::clicked, [this](){emit signClose(id_);});
    }
signals:   
    void signSave(size_t,bool);
    void signClose(size_t);

public:    
    size_t id_;
    QCheckBox* isSave_;

};


class CEditGptRoleWnd : public QDialog
{
    Q_OBJECT

public:
    explicit CEditGptRoleWnd(QWidget *parent = nullptr)
    {
        roleNameLineEdit = new QLineEdit(this);
        systemPormpt = new QTextEdit(this);
        tempLineEdit = new QLineEdit(this);       
        buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
        asisstant = new QRadioButton("使用助手", this);
        auto label = new QLabel("角色名字:",this);
        QFormLayout *layout = new QFormLayout(this);
        QHBoxLayout *layout1 = new QHBoxLayout(this);
        layout1->addWidget(label);
        layout1->addWidget(roleNameLineEdit);
        layout1->addWidget(asisstant);
        auto label1 = new QLabel("活跃度:",this);
        layout1->addWidget(label1);
        layout1->addWidget(tempLineEdit);
        // layout->addRow(label, roleNameLineEdit,radioButton);
        layout->addRow(layout1);
        layout->addRow("系统命令:", systemPormpt);
        // layout->addRow("活跃度:", tempLineEdit);
        layout->addWidget(buttonBox);
        // setFixedWidth(450);
        // descriptionLineEdit->setFixedHeight(150);
        connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }

    QLineEdit *roleNameLineEdit;
    QTextEdit *systemPormpt;
    QLineEdit *tempLineEdit;
    QRadioButton *asisstant;
    QDialogButtonBox *buttonBox;
};


