#pragma once

#include <QObject>
#include "UMessage.h"

class COpenAI;
class CGpts:public QObject
{
    Q_OBJECT
public:
    CGpts(QObject *parent = nullptr);
    ~CGpts(){}    

    std::pair<QStringList, QStringList>  getGptWndData();
    void askGpt(UMessage& msg);
    std::map<QString,QString> roleInfo(const QString& name);
    void delRole(const QString& );
    void saveMsg(const QString& fileName);
    void saveRoleInfo(const QString&,const std::map<QString,QString>&);
    void loadMsg(const QString &fileName);
    
    void slotOaiReply(const QString &text,int statusCode);
    void slotMsgWndSaveClick(size_t id,bool isSave);
    void slotMsgWndcloseClick(size_t id);
    

signals:  
    void signGptReply(const UMessage& msg);
    void signStatusLeftText(const QString& text);
    void signStatusRightText(const QString& text);

public:
    QVector<UMessage> messages_{};

private:
    QByteArray createOiaMsgV(const QImage &, const QString &);
    QByteArray createOiaMeg(const QString &, const QString &, const QString &);

private:
    std::map<QString,QString> modelsName_{};
    std::map<QString,std::map<QString,QString>> rolesInfo_{};
    COpenAI *openai_;    
    float money_{0}; 
};