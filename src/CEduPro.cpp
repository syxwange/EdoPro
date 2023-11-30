#include "CEduPro.h"
#include "core/CScreenCapture.h"
#include "wnd/CMainWnd.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <ranges>
#include <QFile>


namespace  views = std::ranges::views ;

/*
    "usage": {
        "prompt_tokens": 9,
        "completion_tokens": 12,
        "total_tokens": 21
      }

      {
  "id": "chatcmpl-8QX8v3utgc1lQlYHvuqtb0RG2AoA9",
  "object": "chat.completion",
  "created": 1701334621,
  "model": "gpt-3.5-turbo-1106",
  "choices": [
    {
      "index": 0,
      "message": {
        "role": "assistant",
        "content": "Hello! How can I assist you today?"
      },
      "finish_reason": "stop"
    }
  ],
  "usage": {
    "prompt_tokens": 37,
    "completion_tokens": 9,
    "total_tokens": 46
  },
  "system_fingerprint": "fp_eeff13170a"
}

*/

CEduPro::CEduPro():openai_(new COpenAI)
{
    init();
    connect(openai_,&COpenAI::sigOaiReply,this,&CEduPro::slotOaiReply);
}

void CEduPro::show()
{
    win_ = new CMainWnd;
    win_->show();
}

QStringList CEduPro::getDataForWnd(const QString &name)
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
    return childGroups;
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
        QString errorText = QString::number(statusCode)+": "+text;
        emit signStatusText(errorText.replace("\n","").left(130));
        emit signMultiOaiReply();     
        return;
    }
    auto root =  QJsonDocument::fromJson(text.toUtf8()).object();
	auto content = root["choices"][0]["message"]["content"].toString(); 
    if (root["model"].toString().contains("gpt-4")){ 
        money_+= root["usage"].toObject()["prompt_tokens"].toInt()*0.00001;   
        money_ += root["usage"].toObject()["completion_tokens"].toInt()*0.00003; 
    }
    else{
        money_+= root["usage"].toObject()["prompt_tokens"].toInt()*0.000001;   
        money_ += root["usage"].toObject()["completion_tokens"].toInt()*0.000002; 
    }     
    UMesaage message;
    message.index = messages_.size();
    message.isSave = false;
    message.isShow = true;        
    message.content = content;
    message.owner = currentRole_; 
    messages_.push_back(message);  
        
    emit signMultiOaiReply();    
    emit signStatusText(currentRole_+" : OK");
    emit signStatusRightText("金额："+QString::number(money_,'f',4)+"美元");
}

void CEduPro::multiAskGpt()
{     
    QString text{};
    currentRole_ = messages_.last().receiver;
    for (auto& message:messages_|views::filter([&](auto i){return i.isShow;})) 
        text+=message.content+"\n"; 
    auto gptMessage = createOiaMessage(currentRole_, messages_.last().model, text);  
    openai_->chat(gptMessage);
}

void CEduPro::delRole(const QString &name)
{
    QSettings settings("config.ini",QSettings::IniFormat);
    settings.remove(currentTheme_+"/"+name);
}

void CEduPro::saveMsgAsJson(const QString& fileName)
{
    QJsonArray jsonArray;
    for (auto& message:messages_|views::filter([&](auto i){return i.isSave;}))
    {
        QJsonObject json;
        json["index"] = int(message.index);
        json["owner"] = message.owner;
        json["receiver"] = message.receiver;
        json["content"] = message.content;
        jsonArray.append(json);
    }
    QJsonDocument jsonDoc(jsonArray);
    QFile file(fileName);
    if (file.open(QFile::WriteOnly)) {
        file.write(jsonDoc.toJson());
    }
    emit signStatusText("保存成功");
}

void CEduPro::loadMsgFromJson(const QString &fileName)
{
    //从fileName中读入文件，并转化为json,放到messages_变量中   
    QFile file(fileName);
     if (file.open(QFile::ReadOnly)) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
        QJsonArray jsonArray = jsonDoc.array();
        for (const QJsonValue& value : jsonArray) {
            auto tempJson =  value.toObject();
            UMesaage message;
            message.index = tempJson["index"].toInt();
            message.owner = tempJson["owner"].toString();
            message.receiver = tempJson["receiver"].toString();
            message.content = tempJson["content"].toString();
            message.isSave = true;
            message.isShow = true;
            messages_.push_back(message);
        }
    }
}

void CEduPro::slotAskGpt(const QString &roleName, const QString &modelName, const QString &userPrompt)
{   
    currentRole_ = roleName;
    auto message = createOiaMessage(roleName,modelName, userPrompt);
    openai_->chat(message);      
}

void CEduPro::saveRole(const QString &name, const QString &systemPrompt, const QString &temperature)
{
    rolesInfo_[name]=QStringList {systemPrompt,temperature};

    QSettings settings("config.ini",QSettings::IniFormat);
    settings.beginGroup(currentTheme_);
    settings.beginGroup(name);

    settings.setValue("systemPrompt",systemPrompt );
    settings.setValue("temperature",temperature);

    settings.endGroup();  // end "math" group
    settings.endGroup();      
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
}
