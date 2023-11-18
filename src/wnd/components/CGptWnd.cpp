#include "CGptWnd.h"

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

const QString btnStyle="QPushButton {background-color: #f8fcfe;color: #02114d;border-radius: 5;}"
                        "QPushButton:hover {background-color: #7ab5d4;color: #02114d;}"
                        "QPushButton{border: none;}";

CGptWnd::CGptWnd(QWidget *parent,Qt::Orientation orientation):QSplitter(orientation,parent)
{
 
    setStyleSheet("QSplitter::handle{background-color: #f8fcfe;}"); 
    setMouseTracking(true);
    setHandleWidth(3);
    
    auto gtpsWnd = new QWidget(this);
    gtpsWnd->setMouseTracking(true);
    gtpsWnd->setStyleSheet("background-color: #f6f6f6;");
    gtpsWnd->setMaximumWidth(170);
    gtpsWnd->setMinimumWidth(160);

    auto addfileBtn = new QPushButton("上传文件",gtpsWnd);
    addfileBtn->setStyleSheet(btnStyle);
    addfileBtn->setFixedSize(140,40);
    connect(addfileBtn,&QPushButton::clicked,this,&CGptWnd::addFile);
  
    auto addBtn = new QPushButton("添加自定义GPT",gtpsWnd);
    addBtn->setStyleSheet(btnStyle);
    addBtn->setFixedSize(140,40);
    connect(addBtn,&QPushButton::clicked,this,&CGptWnd::addRoleBtn);

 
    gtpsLayout_ = new QVBoxLayout(gtpsWnd);
    gtpsLayout_->addStretch();
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
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::ALT + Qt::Key_Return), askBtn_);
    QObject::connect(shortcut, &QShortcut::activated, askBtn_, &QPushButton::click);

    askBtn_->setStyleSheet("QPushButton {background-color: #7ab5d4;color: #02114d;border-radius: 5;border: none;}");
    askBtn_->setFixedHeight(40); 
    askBtn_->setMaximumWidth(120);
    askBtn_->setMinimumWidth(100);
    modelsLayout_ = new QHBoxLayout(contrlWnd);
    modelsLayout_->addStretch();
    modelsLayout_->addWidget(askBtn_,0,Qt::AlignCenter);  
    modelsLayout_->setContentsMargins(20,0,20,0);
    connect(askBtn_,&QPushButton::clicked,this,&CGptWnd::askBtnClicked);
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

CGptWnd::~CGptWnd()
{

}


void CGptWnd::roleBtnClicked()
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

void CGptWnd::createGptRole(const QString &text)
{
    auto tempBtn = new QPushButton(text,this);
    tempBtn->setStyleSheet(btnStyle);
    tempBtn->setFixedSize(140,40);  
    gtpsLayout_->insertWidget(gtpsLayout_->count()-3,tempBtn,0,Qt::AlignHCenter);
    connect(tempBtn,&QPushButton::clicked,this,&CGptWnd::roleBtnClicked);
    roleBtns_.push_back(tempBtn);
}

void CGptWnd::addRoleBtn()
{
    CInputGptRoleWndg dialog(this);
    connect(&dialog, &QDialog::accepted, [this, &dialog]() {
        auto roleName = dialog.roleNameLineEdit->text();
        auto description = dialog.descriptionLineEdit->toPlainText();
        auto temp = dialog.tempLineEdit->text();        
        if (dialog.asisstant->isChecked())
            roleName+="*";
        createGptRole(roleName);
        emit signAddGptRole(roleName,description,temp);
    }); 
    dialog.exec(); 
}

void CGptWnd::slotOaiReply(const QString &text)
{
    auto message = new CMessageWnd( 0,"🤖 "+text,CMessageWnd::ROBOT);    
    scrollLayout_->insertWidget(scrollLayout_->count()-1, message); 
    messageWnds_.push_back(message);
    askBtn_->setEnabled(true); 
    askBtn_->setText("提 交");
}

void CGptWnd::askBtnClicked()
{    
    auto text = textEdit_->toPlainText();
    CMessageWnd * message = new CMessageWnd( 0, "👨 "+text,CMessageWnd::MAN);    
    if (!isChatLive_)
    {          
        for(auto& messageWnd : messageWnds_) 
        {
            messageWnd->close();
        }
        messageWnds_.clear();    
    }   
    scrollLayout_->insertWidget(scrollLayout_->count()-1, message);
    messageWnds_.push_back(message); 
    askBtn_->setDisabled(true);
    askBtn_->setText("等待中..."); 
    textEdit_->clear();
    textEdit_->setFocus();     
    emit signAskGpt(roleName_,modelName_,text);
}

void CGptWnd::slotRolesName(const QStringList &rolesName)
{
    for (auto& role : rolesName)
    {
        createGptRole(role);
    }
    roleBtns_[0]->click();
}

void CGptWnd::modelBtnClicked()
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

void CGptWnd::addFile()
{
}

void CGptWnd::createModelBtn(const QString &text)
{
    auto tempBtn = new QPushButton(text,this);
    tempBtn->setStyleSheet(btnStyle);
    tempBtn->setFixedHeight(40); 
    tempBtn->setFixedWidth(70);
    qDebug()<<modelsLayout_->count(); 
    modelsLayout_->insertWidget(modelsLayout_->count()-2,tempBtn,0,Qt::AlignCenter);
    connect(tempBtn,&QPushButton::clicked,this,&CGptWnd::modelBtnClicked);
    modelBtns_.push_back(tempBtn);
}

void CGptWnd::addModels(const QStringList &models)
{
    for (auto& model : models)
    {
        createModelBtn(model);
    }
}


