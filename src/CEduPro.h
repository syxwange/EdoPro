#ifndef CEDUPRO_H
#define CEDUPRO_H

#pragma once
#include <QObject>
#include "openai/COpenAI.h"

class CMainWnd;
class CScreenCapture;
class COpenAI;

class CEduPro:public QObject
{
    Q_OBJECT
public:
    CEduPro();
    ~CEduPro(){}
    void show();
    void slotAskGpt(const QString& roleName,const QString& modelName,const QString& userPrompt);
signals:    

private:
    QByteArray createOiaMessage(const QString& roleName,const QString& modelName,const QString& userPrompt);
    void connectSignals();
    void getSettings() ;

    CMainWnd * win_;
    COpenAI * openai_;
    std::map<QString, QStringList> rolesInfo_{};
    std::map<QString,QString> modelsName_{};
};

#endif