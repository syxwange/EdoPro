#include "CMultiGptWnd.h"

#include <QVBoxLayout>
#include "CMessageWnd.h"
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

#include "../../CEduPro.h"

const QString btnStyle="QPushButton {background-color: #f8fcfe;color: #02114d;border-radius: 5;}"
                        "QPushButton:hover {background-color: #7ab5d4;color: #02114d;}"
                        "QPushButton{border: none;}";

CMultiGptWnd::CMultiGptWnd(QWidget *parent,Qt::Orientation orientation):QSplitter(orientation,parent)
{
 
    setStyleSheet("QSplitter::handle{background-color: #f8fcfe;}"); 
    setMouseTracking(true);
    setHandleWidth(3);
    
    auto gtpsWnd = new QWidget(this);
    gtpsWnd->setMouseTracking(true);
    gtpsWnd->setStyleSheet("background-color: #f6f6f6;");
    gtpsWnd->setMaximumWidth(170);
    gtpsWnd->setMinimumWidth(160);

    auto showBtn = new QPushButton("显示完整对话",gtpsWnd);
    showBtn->setStyleSheet(btnStyle);
    showBtn->setFixedSize(140,40);
    connect(showBtn,&QPushButton::clicked,this,&CMultiGptWnd::showMessage);

    auto saveBtn = new QPushButton("保存对话",gtpsWnd);
    saveBtn->setStyleSheet(btnStyle);
    saveBtn->setFixedSize(140,40);
    connect(saveBtn,&QPushButton::clicked,this,&CMultiGptWnd::saveMessage);

    auto addfileBtn = new QPushButton("打开文件",gtpsWnd);
    addfileBtn->setStyleSheet(btnStyle);
    addfileBtn->setFixedSize(140,40);
    connect(addfileBtn,&QPushButton::clicked,this,&CMultiGptWnd::addFile);
  
    auto addBtn = new QPushButton("添加自定义GPT",gtpsWnd);
    addBtn->setStyleSheet(btnStyle);
    addBtn->setFixedSize(140,40);
    connect(addBtn,&QPushButton::clicked,[this](){addRole();});

 
    gtpsLayout_ = new QVBoxLayout(gtpsWnd);
    gtpsLayout_->addStretch();
    gtpsLayout_->addWidget(showBtn,0,Qt::AlignHCenter);
    gtpsLayout_->addWidget(addfileBtn,0,Qt::AlignHCenter);
    gtpsLayout_->addWidget(saveBtn,0,Qt::AlignHCenter);
    gtpsLayout_->addWidget(addBtn,0,Qt::AlignHCenter);
    gtpsLayout_->setSpacing(15);

    //////////////////////////////////////////////////////////////

    auto chatWnd = new QWidget(this);
    chatWnd->setStyleSheet("background-color: #edf7fc;");
    auto chatLayout = new QVBoxLayout(chatWnd);

    scrollArea_ = new QScrollArea(chatWnd);
    QWidget *scrollContent = new QWidget(scrollArea_);
    scrollContent->setStyleSheet("background-color:#fbfbfb;");
    scrollLayout_ = new QVBoxLayout(scrollContent);     
    scrollLayout_->addStretch();   

    scrollArea_->setWidget(scrollContent);
    scrollArea_->setWidgetResizable(true);
    scrollArea_->setStyleSheet("QScrollArea { border: none; }");
    
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
    askBtn_->setFixedHeight(40); 
    askBtn_->setMaximumWidth(120);
    askBtn_->setMinimumWidth(100);
    modelsLayout_ = new QHBoxLayout(contrlWnd);
    modelsLayout_->addStretch();
    modelsLayout_->addWidget(askBtn_,0,Qt::AlignCenter);  
    modelsLayout_->setContentsMargins(20,0,20,0);
    connect(askBtn_,&QPushButton::clicked,this,&CMultiGptWnd::askBtnClicked);
    contrlWnd->setFixedHeight(50);

    chatLayout->addWidget(scrollArea_);
    chatLayout->addWidget(textEdit_);
    chatLayout->addWidget(contrlWnd);
    chatLayout->setStretch(0, 7); // 第一个窗口占70%（7/10）
    chatLayout->setStretch(1, 2); // 第二个窗口占20%（2/10）
    chatLayout->setContentsMargins(0,0,0,0);
    chatLayout->setSpacing(3);

    addWidget(gtpsWnd);
    addWidget(chatWnd);

    addModels({"GPT-3.5","GPT-3.5I","GPT-4","GPT-4V"});
    modelBtns_[0]->click(); 

    ///////////////////////////////////////////////////////////////////////////////       
    // initRoleBtn("video");
}

CMultiGptWnd::~CMultiGptWnd(){}

void CMultiGptWnd::roleBtnClicked()
{
    auto senderObj = dynamic_cast<QPushButton*>(sender());
    roleName_ = senderObj->text(); 
    senderObj->setStyleSheet("QPushButton {background-color: #7ab5d4;color: #02114d;border-radius: 5;}" );
    for (const auto& btn : roleBtns_)
    {
        if (btn != senderObj) 
            btn->setStyleSheet(btnStyle);
    }
}

void CMultiGptWnd::createRoleBtn(const QString &text)
{
    auto tempBtn = new CMenuButton(text,this);
    tempBtn->setStyleSheet(btnStyle);
    tempBtn->setFixedSize(140,40);  
    gtpsLayout_->insertWidget(gtpsLayout_->count()-5,tempBtn,0,Qt::AlignHCenter);
    connect(tempBtn,&CMenuButton::clicked,this,&CMultiGptWnd::roleBtnClicked);
    connect(tempBtn,&CMenuButton::signDelClicked,this,&CMultiGptWnd::roleBtnDelClicked);
    connect(tempBtn,&CMenuButton::signEditClicked,this,&CMultiGptWnd::roleBtnEditClicked);
    roleBtns_.push_back(tempBtn);
}

void CMultiGptWnd::addRole(const QString & name,const QStringList& info,bool isEdit)
{
    CInputGptRoleWndg dialog(this);
    if (isEdit)
    {
        dialog.roleNameLineEdit->setText(name);
        dialog.systemPormpt->setText(info[0]);
        dialog.tempLineEdit->setText(info[1]);
    }
    connect(&dialog, &QDialog::accepted, [this, &dialog,isEdit]() {
        auto roleName = dialog.roleNameLineEdit->text();
        auto description = dialog.systemPormpt->toPlainText();
        auto temp = dialog.tempLineEdit->text();   
        if (!isEdit)
        {
            if (dialog.asisstant->isChecked())
                roleName+="*";
            createRoleBtn(roleName);
        }   
        CEduPro::app()->saveRole(roleName,description,temp);
    }); 
    dialog.exec(); 
    
}

void CMultiGptWnd::slotMultiOaiReply()
{   
    for (auto&wnd:messageWnds_)
        wnd->close();
    messageWnds_.clear();
    for (auto& item :CEduPro::app()->messages_|std::views::filter([&](auto i){return i.isShow;}))
    {
        auto message = new CMultiMessageWnd(item.index,item.content ,mapColors_[item.owner],item.owner, this);        
        message->isSave_->setChecked(item.isSave);
        connect(message,&CMultiMessageWnd::signChange,this,&CMultiGptWnd::messageWndClicked);
        scrollLayout_->insertWidget(scrollLayout_->count()-1, message); 
        messageWnds_.push_back(message);
       
    }  
    askBtn_->setEnabled(true); 
    askBtn_->setText("提 交");
    QApplication::processEvents(); 
    QTimer::singleShot(0.1, [=]() {
        QScrollBar* scrollbar = scrollArea_->verticalScrollBar();
        scrollbar->setValue(scrollbar->maximum());
    });
}

void CMultiGptWnd::initRoleBtn(const QString& name)
{    
    auto rolesNames = CEduPro::app()->getDataForWnd(name);
    for (int i =0;i<rolesNames.size();i++)
    {
        createRoleBtn(rolesNames[i]); 
        int n = i<colors_.size()?i:colors_.size()-1;
        mapColors_[rolesNames[i]]=colors_[n];
    }  
    if (roleBtns_.size()>0) 
        roleBtns_.at(0)->click();
}

void CMultiGptWnd::askBtnClicked()
{
    if (roleName_=="CHATGPT"||roleName_=="英文翻译")
        CEduPro::app()->messages_.clear();
    auto text = textEdit_->toPlainText(); 
    if (text.isEmpty())
    {
        if (CEduPro::app()->messages_.size()>0)
        {
            CEduPro::app()->messages_.last().receiver = roleName_;
            CEduPro::app()->messages_.last().model = modelName_;
            CEduPro::app()->multiAskGpt();
            askBtn_->setDisabled(true);
            askBtn_->setText("等待中..."); 
            textEdit_->clear();
            textEdit_->setFocus();
        }        
        return;
    }  
    CMultiMessageWnd * wnd = new CMultiMessageWnd(messageWnds_.size(), text,mapColors_["man"],"man", this);
    connect(wnd,&CMultiMessageWnd::signChange,this,&CMultiGptWnd::messageWndClicked);
    CEduPro::app()->messages_.append({static_cast<size_t>(CEduPro::app()->messages_.count()),false,true,"man",roleName_,modelName_,text});
    messageWnds_.push_back(wnd);
    
    scrollLayout_->insertWidget(scrollLayout_->count()-1, wnd);     
    askBtn_->setDisabled(true);
    askBtn_->setText("等待中..."); 
    textEdit_->clear();
    textEdit_->setFocus();
    CEduPro::app()->multiAskGpt();
}

void CMultiGptWnd::modelBtnClicked()
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

void CMultiGptWnd::addFile()
{
    //QT打开文件对话框打开json文件，并放到QVector<UMesaage>中
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开对话"), ".", tr("json Files(*.json)"));
    if (fileName.isEmpty())
        return;
    CEduPro::app()->loadMsgFromJson(fileName);
    showMessage();
}

void CMultiGptWnd::showMessage()
{
    for (auto& wnd:messageWnds_)
        wnd->close();
    messageWnds_.clear();
    for (auto& item : CEduPro::app()->messages_)
    {
        if (!mapColors_.contains(item.owner)){
            QMessageBox::warning(nullptr, "wanring", "角色--"+item.owner+"--不存在，请更换场景窗口！");
            return;
        }
        auto message = new CMultiMessageWnd(item.index,item.content ,mapColors_[item.owner],item.owner, this);
        message->isShow_->setChecked(item.isShow);
        message->isSave_->setChecked(item.isSave);
        connect(message,&CMultiMessageWnd::signChange,this,&CMultiGptWnd::messageWndClicked);
        scrollLayout_->insertWidget(scrollLayout_->count()-1, message); 
        messageWnds_.push_back(message);
        QApplication::processEvents(); 
    }
   
    
    QTimer::singleShot(0.1, [=]() {
        QScrollBar* scrollbar = scrollArea_->verticalScrollBar();
        scrollbar->setValue(scrollbar->maximum());
    });
  
}

void CMultiGptWnd::createModelBtn(const QString &text)
{
    auto tempBtn = new QPushButton(text,this);
    tempBtn->setStyleSheet(btnStyle);
    tempBtn->setFixedHeight(40); 
    tempBtn->setFixedWidth(70);
    qDebug()<<modelsLayout_->count(); 
    modelsLayout_->insertWidget(modelsLayout_->count()-2,tempBtn,0,Qt::AlignCenter);
    connect(tempBtn,&QPushButton::clicked,this,&CMultiGptWnd::modelBtnClicked);
    modelBtns_.push_back(tempBtn);
}

void CMultiGptWnd::saveMessage()
{
    //打开保存文件对话框
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存对话"), ".", tr("json Files(*.json)"));
    if (fileName.isEmpty())
        return;
    CEduPro::app()->saveMsgAsJson(fileName);
}

void CMultiGptWnd::messageWndClicked()
{
    auto senderObj = dynamic_cast<CMultiMessageWnd*>(sender());
    for (auto& message : CEduPro::app()->messages_)
    {
        if (message.index == senderObj->id_) 
        {
            message.isShow = senderObj->isShow_->isChecked();
            message.isSave = senderObj->isSave_->isChecked();
        }          
    }
}

void CMultiGptWnd::roleBtnDelClicked()
{
    auto senderObj = dynamic_cast<CMenuButton*>(sender());
    auto roleName = senderObj->text();
    CEduPro::app()->delRole(roleName);
    senderObj->close();
}

void CMultiGptWnd::roleBtnEditClicked()
{
    auto senderObj = dynamic_cast<CMenuButton*>(sender());
    auto roleName = senderObj->text();
    auto roleInfo = CEduPro::app()->rolesInfo_[roleName];
    addRole(roleName,roleInfo,true); 
}

void CMultiGptWnd::addModels(const QStringList &models)
{
    for (auto& model : models)
    {
        createModelBtn(model);
    }
}


