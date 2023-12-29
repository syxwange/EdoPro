#include "CGpts.h"
#include <QSettings>
#include "../openai/COpenAI.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <ranges>
#include <QBuffer>
#include <QFile>
#include <map>

namespace  views = std::ranges::views ;

CGpts::CGpts(QObject *parent):openai_(new COpenAI)
{
    connect(openai_,&COpenAI::sigOaiReply,this,&CGpts::slotOaiReply);
}

std::pair<QStringList, QStringList>  CGpts::getGptWndData()
{
    QSettings settings("config.ini",QSettings::IniFormat);
    
    settings.beginGroup("models");
    QStringList modelBtnNames = settings.childKeys();
    for (const auto& key : modelBtnNames)
    {
        modelsName_[key]=settings.value(key).toString();
    }
    settings.endGroup();

    settings.beginGroup("gpts");
    auto roleNames = settings.childGroups();    
    for (const QString& child : roleNames) {
        settings.beginGroup(child);
        std::map<QString,QString> temp{};
        QStringList childKeys = settings.childKeys();
        for (const auto& key : childKeys) {              
            temp[key]=settings.value(key).toString();    
        }
        rolesInfo_[child]=temp;
        settings.endGroup();
    }
    settings.endGroup();
    // for(const auto&key :rolesInfo_)
    // {
    //     for (auto& [key1,value1]:rolesInfo_[key.first])
    //     {
    //       qDebug()<<key1;
    //       qDebug()<<value1;
    //     }
    // }
    return std::make_pair(modelBtnNames,roleNames);
}

QByteArray CGpts::createOiaMeg(const QString &roleName, const QString &modelName, const QString &prompt)
{
    QJsonObject root;
    root.insert("model",modelsName_[modelName]);    
    root.insert("temperature",rolesInfo_[roleName]["temperature"].toFloat());
    QJsonArray messages;
    messages.append(QJsonObject({ {"role","system"},{"content",rolesInfo_[roleName]["systemPrompt"]}}));
    messages.append(QJsonObject({ {"role","user"},{"content",prompt}}));
    root.insert("messages",messages);
    QJsonDocument doc(root);
    return doc.toJson();
}

QByteArray CGpts::createOiaMsgV(const QImage &image, const QString &prompt)
{

    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    // 打开缓冲区并将图片保存到其中
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG"); // 或者"JPG"

    // 将字节数组转换为Base64格式
    QString base64Image = byteArray.toBase64();  
    QJsonObject root;
    root.insert("model","gpt-4-vision-preview"); 
    QJsonArray messages;
    QJsonArray content;
    content.append(QJsonObject({ {"type","text"},{"text",prompt}}));
    QJsonObject imgJson = QJsonObject{{"url", "data:image/jpeg;base64,"+base64Image}};
    content.append(QJsonObject({ {"type","image_url"},{"image_url",imgJson}}));
    messages.append(QJsonObject({ {"role","user"},{"content",content}}));
    root.insert("messages",messages);
    QJsonDocument doc(root);
    return doc.toJson();
}

void CGpts::slotOaiReply(const QString &text,int statusCode)
{
    
    if (statusCode!=200)
    {
        QString errorText = QString::number(statusCode)+": "+text;
        emit signStatusLeftText(errorText.replace("\n","").left(130));
        emit signGptReply(UMessage{});     
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
    UMessage message;
    message.index = messages_.size();
    message.isSave = false;          
    message.content = content;
    message.owner = messages_.last().receiver; 
    if (rolesInfo_[message.owner]["chat"]=="1")
        message.isShow = true;
    else
        message.isShow = false;  
    messages_.push_back(message);      
        
    emit signGptReply(message);    
    emit signStatusLeftText(message.owner+" : OK");
    emit signStatusRightText("金额："+QString::number(money_,'f',4)+"美元");
}

void CGpts::askGpt(UMessage &msg)
{
    QString text{},apiKey{};
    bool isNew = true;
    auto role = msg.receiver;    
    auto modelName = msg.model;
    if (modelName=="google")
        apiKey = "sk-google";
    else if (modelName=="googleV")
        apiKey = "sk-googlev";
    else
        apiKey = "sk-syxwange";
    if (modelName=="GPT-4V"||modelName=="googleV")
    {  
        auto gptMessage = createOiaMsgV(msg.image, msg.content);
        messages_.push_back(msg);  
        openai_->chat(gptMessage,apiKey);
        return;
    }
    if (msg.content.isEmpty())    {
        msg = messages_.last();
        msg.receiver = role;
        msg.model = modelName; 
        isNew = false;     
    }       
    if (rolesInfo_[msg.receiver]["chat"]=="1")
    {
        for (auto& message:messages_|views::filter([&](auto i){return i.isShow;})) 
            text+=message.content+"\n"; 
        text+=msg.content;  
        msg.isShow = true;
    }else
    {
        text = msg.content;
        msg.isShow = false; 
    }
    if (isNew)
    {
        msg.index = messages_.size();
        messages_.push_back(msg);  
    }
 
    auto gptMessage = createOiaMeg(role, modelName, text);  
    // qDebug()<<gptMessage;
    openai_->chat(gptMessage,apiKey);
}

///////////////////////////////////////////////////////

std::map<QString,QString> CGpts::roleInfo(const QString &name)
{
    return rolesInfo_[name];
}

void CGpts::delRole(const QString& roleName)
{
    QSettings settings("config.ini",QSettings::IniFormat);
    settings.remove("gpts/"+roleName);
}

void CGpts::saveMsg(const QString& fileName)
{
    QJsonArray jsonArray;
    if (fileName.contains("AutoSave"))
    {
        for (auto& message:messages_)
        {
            QJsonObject json;
            json["index"] = int(message.index);
            json["owner"] = message.owner;
            json["receiver"] = message.receiver;
            json["content"] = message.content;
            jsonArray.append(json);
        }
    }else{
        for (auto& message:messages_|views::filter([&](auto i){return i.isSave;}))
        {
            QJsonObject json;
            json["index"] = int(message.index);
            json["owner"] = message.owner;
            json["receiver"] = message.receiver;
            json["content"] = message.content;
            jsonArray.append(json);
        }
    }
    QJsonDocument jsonDoc(jsonArray);
    QFile file(fileName);
    if (file.open(QFile::WriteOnly)) {
        file.write(jsonDoc.toJson());
    }
    emit signStatusLeftText("保存成功");
}

void CGpts::loadMsg(const QString &fileName)
{
    messages_.clear();
   
    QFile file(fileName);
     if (file.open(QFile::ReadOnly)) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
        QJsonArray jsonArray = jsonDoc.array();
        for (const QJsonValue& value : jsonArray) {
            auto tempJson =  value.toObject();
            UMessage message;
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

void CGpts::slotMsgWndSaveClick(size_t id,bool isSave)
{
    messages_[id].isSave = isSave;
    emit signStatusLeftText("保存成功");
}

void CGpts::slotMsgWndcloseClick(size_t id)
{
    messages_[id].isShow = false;
    emit signStatusLeftText("关闭成功");
}

void CGpts::saveRoleInfo(const QString & name, const std::map<QString,QString> &roleinfo)
{
 
    rolesInfo_[name]=roleinfo;   
    QSettings settings("config.ini",QSettings::IniFormat);
    settings.beginGroup("gpts");
    settings.beginGroup(name);

    settings.setValue("systemPrompt", roleinfo.at("systemPrompt"));
    settings.setValue("temperature",roleinfo.at("temperature"));
    settings.setValue("chat",roleinfo.at("chat"));

    settings.endGroup();  
    settings.endGroup();  
 
}
