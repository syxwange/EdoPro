#pragma once

#include <QPushButton>
#include <QEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QFrame>
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

class CImgButton : public QPushButton
{
    Q_OBJECT
public:
    //
    CImgButton(const QString &normalImagePath, const QString &hoverImagePath, QSize size = QSize(24, 24), QWidget *parent = nullptr)
    : QPushButton(parent), normalImage(normalImagePath), hoverImage(hoverImagePath)
    {
        setStyleSheet("QPushButton{border: none;}"); // 去掉按钮的边框
        setImg(normalImagePath);
        setIconSize(size);
        connect(this, &QPushButton::clicked, [this](){isClicked_ = true;});
    }

    void setImg(const QString &imagePath){setIcon(QIcon(imagePath));}
    void slotSetUnClicked()
    {
        isClicked_ = false;
        setIcon(QIcon(normalImage));
    }

protected:


    void enterEvent(QEnterEvent *event) override
    {
        QPushButton::enterEvent(event);
        setIcon(QIcon(hoverImage));
    }

    void leaveEvent(QEvent *event) override
    {
        QPushButton::leaveEvent(event);
        if (!isClicked_)
            setIcon(QIcon(normalImage));
    }


private:
    QString normalImage;
    QString hoverImage;
    bool isClicked_ = false;
};


class CMenuButton : public QPushButton {
    Q_OBJECT

public:
    CMenuButton(const QString& text,QWidget * parent = nullptr) : QPushButton(text,parent) {}

protected:
    void contextMenuEvent(QContextMenuEvent* event) override {
        QMenu menu(this);

        QAction* action1 = new QAction("edit", this);
        connect(action1, &QAction::triggered, this, &CMenuButton::signEditClicked);
        menu.addAction(action1);

        QAction* action2 = new QAction("del", this);
        connect(action2, &QAction::triggered, this, &CMenuButton::signDelClicked);
        menu.addAction(action2);
        menu.setStyleSheet("QMenu::item:selected { background-color: #A0A0A0; }");
        menu.exec(event->globalPos());   
    }

signals:
    void signEditClicked();
    void signDelClicked();

};

class CMessageWnd:public QFrame
{
    Q_OBJECT

public:
    enum  MessageType
    {
        MAN,
        ROBOT,
        OTHER
    };

    CMessageWnd(int id,QString text,MessageType type, QWidget *parent = nullptr): QFrame(parent),id_(id)
    {       
        //设置背景图片.有人类和机器人两种FEFFEE
        // QString icon;
        switch (type)
        {
        case MAN:
            // icon="./images/man.png";
            setStyleSheet("background-color: #fbffef;border-radius: 10px;color:#032b91;");
            break;
        case ROBOT:
            // icon="./images/robot.png";
            setStyleSheet("background-color: #F3FFEE;border-radius: 10px;");
            break;        
        default:
            break;
        }
        
        //设置文本label 设置字体大小，可换行
        QLabel *textLabel = new QLabel(text, this);
        textLabel->setTextInteractionFlags(textLabel->textInteractionFlags() | Qt::TextSelectableByMouse);
        QFont font("微软雅黑", 13);
        textLabel->setFont(font);
        textLabel->setWordWrap(true);

        //设置关闭图片按钮
        // auto closeBtn = new CImgButton("./images/close-black.png","./images/close-red.png");

        // //设置头像label
        // QLabel* picLabel = new QLabel(this);        
        // picLabel->setPixmap(QPixmap(icon));
        QVBoxLayout *layout = new QVBoxLayout(this);
        // auto hlayout = new QHBoxLayout(this);
        // hlayout->addWidget(picLabel);
        // hlayout->addStretch();
        // hlayout->addWidget(closeBtn);
        
        // layout->addLayout(hlayout);
        layout->addWidget(textLabel);
       

        setLayout(layout);

        //关闭窗口时发送id
        // connect(closeBtn, &QPushButton::clicked, [this](){emit signDelMessageId(id_); });
        //关闭窗口
        // connect(closeBtn, &QPushButton::clicked, this, &CMessageWnd::close);
    }
signals:
    void signDelMessageId(int);

private:
    int id_;
};



class CMultiMessageWnd:public QFrame
{
    Q_OBJECT

public:    

    CMultiMessageWnd(size_t id,QString text, QVector<QString> rgbColor, QString role,QWidget *parent = nullptr): QFrame(parent),id_(id)
    {       
        
        setStyleSheet(QString("background-color:%1;border-radius: 10px;color:%2;").arg(rgbColor[0],rgbColor[1]));
        text_ = text;
        //设置文本label 设置字体大小，可换行
        textLabel_ = new QLabel(text, this);
        textLabel_->setTextInteractionFlags(textLabel_->textInteractionFlags() | Qt::TextSelectableByMouse);
        QFont font("微软雅黑", 13);
        textLabel_->setFont(font);        
        textLabel_->setWordWrap(true);

        //设置关闭图片按钮
        auto closeBtn = new CImgButton("./images/close-black.png","./images/close-red.png");
        
        QVBoxLayout *layout = new QVBoxLayout(this);   
        QHBoxLayout* titleLayout = new QHBoxLayout;
        // //设置头像label
        QLabel* nameLabel = new QLabel("[ "+role+ " ]", this);  
        QFont fontName("微软雅黑", 13);
        nameLabel->setStyleSheet(QString("QLabel { color : %1; }").arg(rgbColor[2]));
        fontName.setBold(true);        
        nameLabel->setFont(fontName); 
        // titleLayout->setContentsMargins(30,0,30,0);
        titleLayout->addWidget(nameLabel);       
        isShow_ = new QCheckBox("显示", this);
        isShow_->setChecked(true);
        isSave_ = new QCheckBox("保存", this);
        titleLayout->addStretch();
        titleLayout->addWidget(isShow_);
        titleLayout->addWidget(isSave_);
        titleLayout->addWidget(closeBtn);    
           
        
        layout->addLayout(titleLayout);
        layout->addWidget(textLabel_);           

        setLayout(layout);

        connect(isShow_, &QCheckBox::clicked, this, &CMultiMessageWnd::signChange);
        connect(isSave_, &QCheckBox::clicked, this, &CMultiMessageWnd::signChange);
        //关闭窗口时发送id
        connect(closeBtn, &QPushButton::clicked, [this](){ isShow_->setChecked(false); emit signChange();});
        //关闭窗口
        connect(closeBtn, &QPushButton::clicked, this, &CMessageWnd::close);
    }
signals:
    void signChange();
  

public:
    void setText(const QString& text){textLabel_->setText(text_+"\n"+text);}

    size_t id_;
    QString text_;
    QCheckBox *isShow_,*isSave_;
    QLabel *textLabel_;
};




class CInputGptRoleWndg : public QDialog
{
    Q_OBJECT

public:
    explicit CInputGptRoleWndg(QWidget *parent = nullptr)
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

