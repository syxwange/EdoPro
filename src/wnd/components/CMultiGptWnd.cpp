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
    connect(showBtn,&QPushButton::clicked,this,&CMultiGptWnd::signShowSaveMessage);

    auto addfileBtn = new QPushButton("上传文件",gtpsWnd);
    addfileBtn->setStyleSheet(btnStyle);
    addfileBtn->setFixedSize(140,40);
    connect(addfileBtn,&QPushButton::clicked,this,&CMultiGptWnd::addFile);
  
    auto addBtn = new QPushButton("添加自定义GPT",gtpsWnd);
    addBtn->setStyleSheet(btnStyle);
    addBtn->setFixedSize(140,40);
    connect(addBtn,&QPushButton::clicked,this,&CMultiGptWnd::addRoleBtn);

 
    gtpsLayout_ = new QVBoxLayout(gtpsWnd);
    gtpsLayout_->addStretch();
    gtpsLayout_->addWidget(showBtn,0,Qt::AlignHCenter);
    gtpsLayout_->addWidget(addfileBtn,0,Qt::AlignHCenter);
    gtpsLayout_->addWidget(addBtn,0,Qt::AlignHCenter);
    gtpsLayout_->setSpacing(15);

    //////////////////////////////////////////////////////////////

    auto chatWnd = new QWidget(this);
    chatWnd->setStyleSheet("background-color: #edf7fc;");
    auto chatLayout = new QVBoxLayout(chatWnd);

    QScrollArea * scrollArea = new QScrollArea(chatWnd);
    QWidget *scrollContent = new QWidget(scrollArea);
    scrollContent->setStyleSheet("background-color:#fbfbfb;");
    scrollLayout_ = new QVBoxLayout(scrollContent);     
    scrollLayout_->addStretch();   

    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; }");
    
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

    chatLayout->addWidget(scrollArea);
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

void CMultiGptWnd::createGptRole(const QString &text)
{
    auto tempBtn = new CMenuButton(text,this);
    tempBtn->setStyleSheet(btnStyle);
    tempBtn->setFixedSize(140,40);  
    gtpsLayout_->insertWidget(gtpsLayout_->count()-4,tempBtn,0,Qt::AlignHCenter);
    connect(tempBtn,&CMenuButton::clicked,this,&CMultiGptWnd::roleBtnClicked);
    connect(tempBtn,&CMenuButton::signDelClicked,this,&CMultiGptWnd::roleBtnDelClicked);
    connect(tempBtn,&CMenuButton::signEditClicked,this,&CMultiGptWnd::roleBtnEditClicked);
    roleBtns_.push_back(tempBtn);
}

void CMultiGptWnd::addRoleBtn()
{
    CInputGptRoleWndg dialog(this);
    connect(&dialog, &QDialog::accepted, [this, &dialog]() {
        auto roleName = dialog.roleNameLineEdit->text();
        auto description = dialog.descriptionLineEdit->toPlainText();
        auto temp = dialog.tempLineEdit->text();        
        if (dialog.asisstant->isChecked())
            roleName+="*";
        createGptRole(roleName);
        emit signAddRole(roleName,description,temp,dialog.asisstant->isChecked());
    }); 
    dialog.exec(); 
}

void CMultiGptWnd::slotEditRole(const QString &name, QStringList roleInfo)
{
}

void CMultiGptWnd::slotMultiOaiReply(QVector<UMesaage> &messages)
{
    messages_ = messages;
    for (auto&wnd:messageWnds_)
        wnd->close();
    messageWnds_.clear();
    for (auto& item :messages_|std::views::filter([&](auto i){return i.isShow;}))
    {
        auto message = new CMultiMessageWnd(item.index,item.content ,mapColors_[item.owner],item.owner, this);
        connect(message,&CMultiMessageWnd::signChange,this,&CMultiGptWnd::messageWndClicked);
        scrollLayout_->insertWidget(scrollLayout_->count()-1, message); 
        messageWnds_.push_back(message);
    }  
    askBtn_->setEnabled(true); 
    askBtn_->setText("提 交");

}

void CMultiGptWnd::slotReset(const QStringList &rolesNames)
{      
    for (auto& roleBtn:roleBtns_)
        roleBtn->close();

    roleBtns_.clear();
    for (int i =0;i<rolesNames.size();i++)
    {
        createGptRole(rolesNames[i]); 
        mapColors_[rolesNames[i]]=colors_[i];
    }   

    if (roleBtns_.size()>0) 
        roleBtns_.at(0)->click();
 
}

void CMultiGptWnd::askBtnClicked()
{
    auto text = textEdit_->toPlainText(); 
    if (text.isEmpty())
    {
        if (messages_.size()>0)
        {
            messages_.last().receiver = roleName_;
            messages_.last().model = modelName_;
            emit signMultiAskGpt(messages_);
            askBtn_->setDisabled(true);
            askBtn_->setText("等待中..."); 
            textEdit_->clear();
            textEdit_->setFocus();
        }        
        return;
    }
        
    QStringList colors{"#fbffef","black","#02114d"};
    CMultiMessageWnd * wnd = new CMultiMessageWnd(messageWnds_.size(), text,colors,"man", this);
    connect(wnd,&CMultiMessageWnd::signChange,this,&CMultiGptWnd::messageWndClicked);
    messages_.append({static_cast<size_t>(messages_.count()),true,true,"man",roleName_,modelName_,text});
    messageWnds_.push_back(wnd);
    
    scrollLayout_->insertWidget(scrollLayout_->count()-1, wnd);     
    askBtn_->setDisabled(true);
    askBtn_->setText("等待中..."); 
    textEdit_->clear();
    textEdit_->setFocus();
    emit signMultiAskGpt(messages_);
}

void CMultiGptWnd::modelBtnClicked()
{
    auto senderObj = dynamic_cast<QPushButton*>(sender());
    modelName_ = senderObj->text(); 
    if (modelName_.contains("*"))
        isChatLive_=true;
    else
        isChatLive_=false;
    senderObj->setStyleSheet("QPushButton {background-color: #7ab5d4;color: #02114d;border-radius: 5;}" );
    for (const auto& btn : modelBtns_)
    {
        if (btn != senderObj) 
            btn->setStyleSheet(btnStyle);
    }
}

void CMultiGptWnd::addFile()
{
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

void CMultiGptWnd::messageWndClicked()
{
    auto senderObj = dynamic_cast<CMultiMessageWnd*>(sender());
    for (auto& message : messages_)
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
    emit signDelRole(roleName);
}

void CMultiGptWnd::roleBtnEditClicked()
{
    auto senderObj = dynamic_cast<CMenuButton*>(sender());
    auto roleName = senderObj->text();
    emit signGetRoleInfo(roleName);

}

void CMultiGptWnd::addModels(const QStringList &models)
{
    for (auto& model : models)
    {
        createModelBtn(model);
    }
}


