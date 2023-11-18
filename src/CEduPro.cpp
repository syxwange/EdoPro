#include "CEduPro.h"
#include "core/CScreenCapture.h"
#include "wnd/CMainWnd.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>



CEduPro::CEduPro():win_(new CMainWnd),openai_(new COpenAI)
{
    getSettings();
    connectSignals();
}


void CEduPro::show()
{
    win_->show();
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

void CEduPro::slotAskGpt(const QString &roleName, const QString &modelName, const QString &userPrompt)
{
    auto message = createOiaMessage(roleName,modelName, userPrompt);
    openai_->chat(message);
}

void CEduPro::connectSignals()
{
    connect(win_,&CMainWnd::signAskGpt,this,&CEduPro::slotAskGpt);
    connect(openai_,&COpenAI::sigOaiReply,win_,&CMainWnd::signOaiReply);
   
}

void CEduPro::getSettings() 
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
    win_->slotRolesName(childGroups);
}
