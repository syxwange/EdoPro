#pragma once

#include "components/CMoveFramelessWnd.h"
#include "../core/UMessage.h"

class CStatusWnd;
class CTitileWnd;
class CCenterWnd;
class CMainWnd : public  CMoveFramelessWnd
{
    Q_OBJECT
public:
    static CMainWnd* getInstance();

signals:   
    void signAskGpt(const QString& roleName,const QString& modelName,const QString& userPrompt);
    void signOaiReply(const QString& text,size_t index);
    void signAddRole(const QString& name,const QString& systemPrompt,const QString& temperature,bool isAssistant);
    void signCreateWnd(const QString& name);
    void signDataForWnd(const QString& name,const QStringList& datas);
    void signStatusText(const QString&);   
    void signMultiOaiReply(QVector<UMesaage>&);
    void signMultiAskGpt(QVector<UMesaage>& messages);
    void signGetRoleInfo(const QString&  roleName);
    void signDelRole(const QString&  roleName);
    void signReplyForWnd(const QString& roleName,QStringList roleInfo);
 

private:
    CMainWnd(QWidget *parent = 0); 
    CTitileWnd * titleWnd_;
    CCenterWnd * centerWnd_;
    CStatusWnd * statusWnd_;

};