#include "CEduPro.h"
#include "core/CScreenCapture.h"
#include "wnd/CMainWnd.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <ranges>

namespace  views = std::ranges::views ;

CEduPro::CEduPro():win_(CMainWnd::getInstance()),openai_(new COpenAI)
{
    init();
    connectSignals();
}

void CEduPro::show()
{
    win_->show();
}

void CEduPro::slotGetDataForWnd(const QString &name)
{
    if (name=="video") 
        messages_.clear();
    currentTheme_ = name;
    QSettings settings("config.ini",QSettings::IniFormat);
    settings.beginGroup(currentTheme_);
    auto childGroups = settings.childGroups();    
    for (const QString& child : childGroups) {
        settings.beginGroup(child);
        QStringList temp{};
        QStringList childKeys = settings.childKeys();
        for (const auto& key : childKeys) {
            QVariant value = settings.value(key);
            temp.append(value.toString());     
        }
        rolesInfo_[child]=temp;
        settings.endGroup();
    }
    settings.endGroup();
    emit win_->signDataForWnd(currentTheme_, childGroups);
}

QByteArray CEduPro::createOiaMessage(const QString &roleName, const QString &modelName, const QString &userPrompt)
{
    QJsonObject root;
    root.insert("model",modelsName_[modelName]);    
    root.insert("temperature",rolesInfo_[roleName][1].toFloat());
    QJsonArray messages;
    messages.append(QJsonObject({ {"role","system"},{"content",rolesInfo_[roleName][0]}}));
    messages.append(QJsonObject({ {"role","user"},{"content",userPrompt}}));
    root.insert("messages",messages);
    QJsonDocument doc(root);
    return doc.toJson();
}

void CEduPro::slotOaiReply(const QString &text,int statusCode)
{
    if (statusCode!=200)
    {
        emit win_->signStatusText(QString::number(statusCode)+": "+text);
        emit win_->signMultiOaiReply(messages_);
        qDebug()<<statusCode<<text;
        return;
    }
    auto root =  QJsonDocument::fromJson(text.toUtf8()).object();
	auto content = root["choices"][0]["message"]["content"].toString();
    size_t id = 1;
    if (currentTheme_=="video")
    {          
        UMesaage message;
        message.index = messages_.size();
        message.isSave = false;
        message.isShow = true;        
        message.content = content;
        message.owner = currentRole_; 
        messages_.push_back(message);  
          
        emit win_->signMultiOaiReply(messages_);
    }else if(currentTheme_=="gpts")
        emit win_->signOaiReply(content,id);
    emit win_->signStatusText(currentRole_+" : OK");
}

void CEduPro::slotMultiAskGpt(QVector<UMesaage>& messages)
{  
    messages_=messages;
    QString text{};
    currentRole_ = messages.last().receiver;
    for (auto& message:messages|views::filter([&](auto i){return i.isShow;})) 
        text+=message.content+"\n"; 
    auto gptMessage = createOiaMessage(currentRole_, messages.last().model, text);  
    openai_->chat(gptMessage);
}

void CEduPro::slotAskGpt(const QString &roleName, const QString &modelName, const QString &userPrompt)
{   
    currentRole_ = roleName;
    auto message = createOiaMessage(roleName,modelName, userPrompt);
    openai_->chat(message);      
}



void CEduPro::slotAddRole(const QString &name, const QString &systemPrompt, const QString &temperature, bool isAssistant)
{
    QSettings settings("config.ini",QSettings::IniFormat);
    settings.beginGroup(currentTheme_);
    settings.beginGroup(name);

    settings.setValue("systemPrompt",systemPrompt );
    settings.setValue("temperature",temperature);

    settings.endGroup();  // end "math" group
    settings.endGroup();
    QStringList temp{};
    temp.append(systemPrompt);
    temp.append(temperature);
    rolesInfo_[name]=temp;
}



void CEduPro::connectSignals()
{
    connect(win_,&CMainWnd::signAskGpt,this,&CEduPro::slotAskGpt);
    connect(win_,&CMainWnd::signAddRole,this,&CEduPro::slotAddRole);
    connect(win_,&CMainWnd::signCreateWnd,this,&CEduPro::slotGetDataForWnd);
    connect(win_,&CMainWnd::signMultiAskGpt,this,&CEduPro::slotMultiAskGpt);
    connect(win_,&CMainWnd::signGetRoleInfo,[this](const QString& roleName){ win_->signReplyForWnd(roleName,rolesInfo_[roleName]);});

    connect(openai_,&COpenAI::sigOaiReply,this,&CEduPro::slotOaiReply);
   
}

void CEduPro::init() 
{ 
    QSettings settings("config.ini",QSettings::IniFormat);
    
    settings.beginGroup("models");
    QStringList childKeys = settings.childKeys();
    for (const auto& key : childKeys)
    {
        modelsName_[key]=settings.value(key).toString();
    }
    settings.endGroup();

    settings.beginGroup("gpts");
    auto childGroups = settings.childGroups();    
    for (const QString& child : childGroups) {
        settings.beginGroup(child);
        QStringList temp{};
        QStringList childKeys = settings.childKeys();
        for (const auto& key : childKeys) {
            QVariant value = settings.value(key);
            temp.append(value.toString());     
        }
        rolesInfo_[child]=temp;
        settings.endGroup();
    }
    settings.endGroup();
    win_->signDataForWnd(currentTheme_, childGroups);
    emit win_->signStatusText(currentRole_+" : OK");
}
