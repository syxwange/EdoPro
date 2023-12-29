#include "CGptWnd.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QScrollArea>
#include <QTextEdit>
#include <QScrollBar>
#include <QDialog>
#include <QInputDialog>
#include <QShortcut>
#include <ranges>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QApplication>
#include "CMsgWnd.h"
#include "customButton.h"
#include <map>
#include "../../core/CGpts.h"

const QString btnStyle=R"(QPushButton {background-color: #f8fcfe;color: #02114d;border-radius: 5;}
                        QPushButton:hover {background-color: #7ab5d4;color: #02114d;}"
                        QPushButton{border: none;})";


CGptWnd::CGptWnd(CGpts* gpts,QWidget *parent,Qt::Orientation orientation):QSplitter(orientation,parent),gpts_(gpts)
{
 
    setStyleSheet("QSplitter::handle{background-color: #f8fcfe;}"); 
    setMouseTracking(true);
    setHandleWidth(3);
    
    auto gtpsWnd = new QWidget(this);
    QScrollArea *scrollAreaRoleBtn =new QScrollArea(gtpsWnd); 
    auto roleBtnsWnd = new QWidget;
    gtpsLayout_ = new QVBoxLayout(roleBtnsWnd);
    gtpsLayout_->addStretch();
    gtpsLayout_->setSpacing(10);
    scrollAreaRoleBtn->setWidget(roleBtnsWnd);
    scrollAreaRoleBtn->setWidgetResizable(true);  
    // scrollAreaRoleBtn->setStyleSheet("QScrollArea { border: none; }");
    scrollAreaRoleBtn->setStyleSheet(R"(
        QScrollArea { border: none; }
        QScrollBar {border: none;width: 3px;background: #a5a5a5;}
        QScrollBar::handle {border: none;width: 5px;}
        QScrollBar::add-line, QScrollBar::sub-line {border: none;}
    )");
    
    // scrollAreaRoleBtn->setStyleSheet("QScrollBar { border: none; }");
    scrollAreaRoleBtn->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gtpsWnd->setMouseTracking(true);
    gtpsWnd->setStyleSheet("background-color: #f6f6f6;");
    gtpsWnd->setMaximumWidth(170);
    gtpsWnd->setMinimumWidth(160);

    auto showBtn = new QPushButton("显示完整对话",gtpsWnd);
    showBtn->setStyleSheet(btnStyle);
    showBtn->setFixedSize(140,32);
    connect(showBtn,&QPushButton::clicked,this,&CGptWnd::showMessage);

    auto saveBtn = new QPushButton("保存对话",gtpsWnd);
    saveBtn->setStyleSheet(btnStyle);
    saveBtn->setFixedSize(140,32);
    connect(saveBtn,&QPushButton::clicked,this,&CGptWnd::saveMessage);

    auto openfileBtn = new QPushButton("打开文件",gtpsWnd);
    openfileBtn->setStyleSheet(btnStyle);
    openfileBtn->setFixedSize(140,32);
    connect(openfileBtn,&QPushButton::clicked,this,&CGptWnd::openfile);

    auto addfileBtn = new QPushButton("添加附件",gtpsWnd);
    addfileBtn->setStyleSheet(btnStyle);
    addfileBtn->setFixedSize(140,32);
    connect(addfileBtn,&QPushButton::clicked,this,&CGptWnd::addFile);
  
    auto addBtn = new QPushButton("添加自定义GPT",gtpsWnd);
    addBtn->setStyleSheet(btnStyle);
    addBtn->setFixedSize(140,32);
    connect(addBtn,&QPushButton::clicked,[this](){addRole();});

 
    auto gtpsLayout = new QVBoxLayout(gtpsWnd);
    gtpsLayout->addWidget(scrollAreaRoleBtn);
    gtpsLayout->addWidget(addfileBtn,0,Qt::AlignHCenter);
    gtpsLayout->addWidget(showBtn,0,Qt::AlignHCenter);    
    gtpsLayout->addWidget(saveBtn,0,Qt::AlignHCenter);
    gtpsLayout->addWidget(openfileBtn,0,Qt::AlignHCenter);
    gtpsLayout->addWidget(addBtn,0,Qt::AlignHCenter);
    gtpsLayout->setSpacing(10);
    gtpsLayout->setContentsMargins(0,0,0,6);
    addWidget(gtpsWnd);
    //////////////////////////////////////////////////////////////

    auto chatWnd = new QWidget(this);
    chatWnd->setStyleSheet("background-color: #edf7fc;");
    auto chatLayout = new QVBoxLayout(chatWnd);

    scrollArea_ = new QScrollArea(chatWnd);
    QWidget *scrollContent = new QWidget(scrollArea_);
    scrollContent->setStyleSheet("background-color:#fbfbfb;");
    msgWndLaout_ = new QVBoxLayout(scrollContent);     
    msgWndLaout_->addStretch();  
    scrollArea_->setStyleSheet(R"(
        QScrollArea { border: none; }
        QScrollBar {border: none;width: 3px;background: #a5a5a5;}
        QScrollBar::handle {border: none;width: 5px;}
        QScrollBar::add-line, QScrollBar::sub-line {border: none;}
    )"); 

    scrollArea_->setWidget(scrollContent);
    scrollArea_->setWidgetResizable(true);

    
    textEdit_ =  new QTextEdit(chatWnd);
    textEdit_->setStyleSheet("QTextEdit{border: none;background-color: #fbfbfb;padding: 1px;};");
    QFont font("微软雅黑", 13);
    textEdit_->setFont(font);
    textEdit_->setFocus();
    auto contrlWnd = new QWidget(chatWnd);
    contrlWnd->setStyleSheet("background-color:#f6f6f6;");
    askBtn_ = new QPushButton("提  交",contrlWnd);
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::ALT|Qt::Key_Return), askBtn_);
    QObject::connect(shortcut, &QShortcut::activated, askBtn_, &QPushButton::click);

    askBtn_->setStyleSheet("QPushButton {background-color: #7ab5d4;color: #02114d;border-radius: 5;border: none;}");
    askBtn_->setFixedHeight(32); 
    askBtn_->setMaximumWidth(120);
    askBtn_->setMinimumWidth(100);
    modelsLayout_ = new QHBoxLayout(contrlWnd);
    modelsLayout_->addStretch();
    modelsLayout_->addWidget(askBtn_,0,Qt::AlignCenter);  
    modelsLayout_->setContentsMargins(20,0,20,0);
    connect(askBtn_,&QPushButton::clicked,this,&CGptWnd::askBtnClick);
    contrlWnd->setFixedHeight(50);

    chatLayout->addWidget(scrollArea_);
    chatLayout->addWidget(textEdit_);
    chatLayout->addWidget(contrlWnd);
    chatLayout->setStretch(0, 7); // 第一个窗口占70%（7/10）
    chatLayout->setStretch(1, 2); // 第二个窗口占20%（2/10）
    chatLayout->setContentsMargins(0,0,0,0);
    chatLayout->setSpacing(3);
    
    addWidget(chatWnd);  

    init();
}

void CGptWnd::init()
{
    auto [models, roleNames] = gpts_->getGptWndData();
    for (int i =0;i<roleNames.size();i++)
    {
        createRoleBtn(roleNames[i]); 
        int n = i<colors_.size()?i:colors_.size()-1;
        mapColors_[roleNames[i]]=colors_[n];
    }  
    if (roleBtns_.size()>0) 
        roleBtns_.at(0)->click();
    
    for (auto& model : models)
    {
        createModelBtn(model);
    }
    modelBtns_[0]->click(); 
    connect(gpts_,&CGpts::signGptReply,this,&CGptWnd::slotGptReply);
}

void CGptWnd::addFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开对话"), ".", tr("image Files(*.png *.jpg *.jpeg)"));
    if (fileName.isEmpty())
        return;
    QImage image(fileName);
    int newWidth, newHeight;
    if (image.width() > image.height()) {
        newWidth = 512;
        newHeight = 512 * image.height() / image.width();
    } else {
        newHeight = 512;
        newWidth = 512 * image.width() / image.height();
    }

    image_ = image.scaled(newWidth, newHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void CGptWnd::slotGptReply(const UMessage& msg)
{   
    if (!msg.content.isEmpty()) 
    {
        auto msgWnd = new CMsgWnd(msg.index,msg.content ,mapColors_[msg.owner],msg.owner,{}, this);        
        msgWnd->isSave_->setChecked(msg.isSave);
        connect(msgWnd,&CMsgWnd::signSave,gpts_,&CGpts::slotMsgWndSaveClick);
        connect(msgWnd,&CMsgWnd::signClose,gpts_,&CGpts::slotMsgWndcloseClick);
        msgWndLaout_->insertWidget(msgWndLaout_->count()-1, msgWnd); 
        msgWnds_.push_back(msgWnd);  
    }

    askBtn_->setEnabled(true); 
    askBtn_->setText("提 交");
    QApplication::processEvents(); 
    QTimer::singleShot(0.1, [=]() {
        QScrollBar* scrollbar = scrollArea_->verticalScrollBar();
        scrollbar->setValue(scrollbar->maximum());
    });
}

void CGptWnd::askBtnClick()
{
    auto text = textEdit_->toPlainText(); 
    bool isShow = true;
    if (gpts_->roleInfo(roleName_)["chat"]=="0")
    {        
        for(auto& wnd :msgWnds_)
            wnd->close();
        msgWnds_.clear();
        isShow=false;
    }
    UMessage message{size_t(gpts_->messages_.size()),false,isShow,"man",roleName_,modelName_,text,image_}; 
    if (!text.isEmpty())
    {
        CMsgWnd * msgWnd = new CMsgWnd(message.index, text, mapColors_["man"],"man", image_, this);
        connect(msgWnd,&CMsgWnd::signSave,gpts_,&CGpts::slotMsgWndSaveClick);
        connect(msgWnd,&CMsgWnd::signClose,gpts_,&CGpts::slotMsgWndcloseClick);        
        msgWndLaout_->insertWidget(msgWndLaout_->count()-1, msgWnd); 
        msgWnds_.push_back(msgWnd); 
    }          
    askBtn_->setDisabled(true);
    askBtn_->setText("等待中..."); 
    textEdit_->clear();
    textEdit_->setFocus();
    gpts_->askGpt(message);   
    image_ = QImage{}; 
}

//////////////////////////////////////////////////////////////

void CGptWnd::createModelBtn(const QString &text)
{
    auto tempBtn = new QPushButton(text,this);
    tempBtn->setStyleSheet(btnStyle);
    tempBtn->setFixedHeight(32); 
    tempBtn->setFixedWidth(70);
    modelsLayout_->insertWidget(modelsLayout_->count()-2,tempBtn,0,Qt::AlignCenter);
    connect(tempBtn,&QPushButton::clicked,this,&CGptWnd::modelBtnClick);
    modelBtns_.push_back(tempBtn);
}

void CGptWnd::modelBtnClick()
{
    auto senderObj = dynamic_cast<QPushButton*>(sender());
    modelName_ = senderObj->text(); 
    senderObj->setStyleSheet("QPushButton {background-color: #7ab5d4;color: #02114d;border-radius: 5;}" );
    for (const auto& btn : modelBtns_)
    {
        if (btn != senderObj) 
            btn->setStyleSheet(btnStyle);
    }
}

void CGptWnd::showMessage(bool isShow)
{    
    for(auto& wnd :msgWnds_)
        wnd->close();
    msgWnds_.clear();
    for (auto& item : gpts_->messages_)
    {
        if (isShow)
        {
            if(!item.isShow)
                continue;
        }
        if (!mapColors_.contains(item.owner)){    
            item.owner="man";
        }
        auto msgWnd = new CMsgWnd(item.index,item.content ,mapColors_[item.owner],item.owner,{}, this);
      
        msgWnd->isSave_->setChecked(item.isSave);
        connect(msgWnd,&CMsgWnd::signSave,gpts_,&CGpts::slotMsgWndSaveClick);
        connect(msgWnd,&CMsgWnd::signClose,gpts_,&CGpts::slotMsgWndcloseClick);
        msgWnds_.push_back(msgWnd);
        msgWndLaout_->insertWidget(msgWndLaout_->count()-1, msgWnd);       
        QApplication::processEvents(); 
    }   
    
    QTimer::singleShot(0.1, [=]() {
        QScrollBar* scrollbar = scrollArea_->verticalScrollBar();
        scrollbar->setValue(scrollbar->maximum());
    });  
}

void CGptWnd::openfile()
{
    //QT打开文件对话框打开json文件，并放到QVector<UMesaage>中
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开对话"), ".", tr("json Files(*.json)"));
    if (fileName.isEmpty())
        return;  
    gpts_->loadMsg(fileName);
    showMessage();
}

void CGptWnd::saveMessage()
{
    //打开保存文件对话框
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存对话"), ".", tr("json Files(*.json)"));
    if (fileName.isEmpty())
        return; 
    gpts_->saveMsg(fileName);
}

void CGptWnd::msgWndSaveClick(size_t id, bool isSave)
{
    gpts_->messages_[id].isSave = isSave;
}

///////////////////////////////////////////////


void CGptWnd::addRole(const QString & name,const std::map<QString,QString>& info,bool isEdit)
{
    CEditGptRoleWnd dialog(this);
    if (isEdit)
    {
        dialog.roleNameLineEdit->setText(name);
        dialog.roleNameLineEdit->setDisabled(true);

        dialog.systemPormpt->setText(info.at("systemPrompt"));
        dialog.tempLineEdit->setText(info.at("temperature"));
        if(info.at("chat")=="1")
            dialog.asisstant->setChecked(true);
        else
            dialog.asisstant->setChecked(false);
    }
    connect(&dialog, &QDialog::accepted, [this, &dialog,isEdit]() {
        std::map<QString,QString> tempmap{};       
        auto roleName = dialog.roleNameLineEdit->text();
        tempmap["systemPrompt"]= dialog.systemPormpt->toPlainText();
        tempmap["temperature"]= dialog.tempLineEdit->text();
        tempmap["chat"]=  dialog.asisstant->isChecked()?"1":"0";   
        if (!isEdit)
        {            
            createRoleBtn(roleName);
        }   
        gpts_->saveRoleInfo(roleName,tempmap);
    }); 
    dialog.exec(); 
    
}

void CGptWnd::createRoleBtn(const QString &text)
{
    auto tempBtn = new CMenuButton(text,this);
    tempBtn->setStyleSheet(btnStyle);
    tempBtn->setFixedSize(140,32);  
    gtpsLayout_->insertWidget(gtpsLayout_->count()-1,tempBtn,0,Qt::AlignHCenter);
    connect(tempBtn,&CMenuButton::clicked,this,&CGptWnd::roleBtnClick);
    connect(tempBtn,&CMenuButton::signDelClicked,this,&CGptWnd::roleBtnDelClick);
    connect(tempBtn,&CMenuButton::signEditClicked,this,&CGptWnd::roleBtnEditClick);
    roleBtns_.push_back(tempBtn);
}

void CGptWnd::roleBtnDelClick()
{
    auto senderObj = dynamic_cast<CMenuButton*>(sender());
    auto roleName = senderObj->text();
    gpts_->delRole(roleName);
    senderObj->close();
}

void CGptWnd::roleBtnEditClick()
{
    auto senderObj = dynamic_cast<CMenuButton*>(sender());
    auto roleName = senderObj->text();
    auto role = gpts_->roleInfo(roleName);
    addRole(roleName,role,true);
}

void CGptWnd::roleBtnClick()
{
    auto senderObj = dynamic_cast<QPushButton*>(sender());
    if (gpts_->roleInfo(roleName_)["chat"]=="0"&&gpts_->roleInfo(senderObj->text())["chat"]=="1")
    {
        for(auto& wnd :msgWnds_)
            wnd->close();
        msgWnds_.clear();
        showMessage(true);
    }
    roleName_ = senderObj->text(); 
    senderObj->setStyleSheet("QPushButton {background-color: #7ab5d4;color: #02114d;border-radius: 5;}" );
    for (const auto& btn : roleBtns_)
    {
        if (btn != senderObj) 
            btn->setStyleSheet(btnStyle);
    }
}

