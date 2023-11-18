#pragma once

#include "components/CMoveFramelessWnd.h"


class CMainWnd : public  CMoveFramelessWnd
{
    Q_OBJECT
public:
    CMainWnd(QWidget *parent = 0); 
    void slotRolesName(const QStringList& roleNames);

signals:   
    void signAskGpt(const QString& roleName,const QString& modelName,const QString& userPrompt);
    void signOaiReply(const QString& text); 
       
private:
    QWidget * titleWnd_{nullptr};
    QWidget * centerWnd_{nullptr};
    QWidget * statusWnd_{nullptr};

};