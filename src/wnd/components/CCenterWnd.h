#ifndef CCENTERWND_H
#define CCENTERWND_H

#pragma once
#include <QWidget>
#include <map>
#include <vector>
#include "../../core/UMessage.h"

class QPushButton;
class CMessageWnd;
class QVBoxLayout;
class QHBoxLayout;
class CImgButton;
class CCenterWnd:public QWidget 
{
    Q_OBJECT
public:
    CCenterWnd(QWidget *parent = nullptr);
    ~CCenterWnd(){}  
    
    void slotCreateWnd(const QString& name,QStringList datas);
signals:
    // void signAddRole(const QString& name,const QString& systemPrompt,const QString& temperature,bool isAssistant);
    // void signAskGpt(const QString& roleName,const QString& modelName,const QString& userPrompt);
    // void signOaiReply(const QString& text,size_t index);


private:   
    void menuBtnClicked();
    void createMenuWnd();

    QHBoxLayout * hLayout_;
    QWidget * leftMenuWnd_{};
    QWidget * rightWnd_{};
    std::vector<CImgButton*> menuBtns_;
};

#endif