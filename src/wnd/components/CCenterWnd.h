#ifndef CCENTERWND_H
#define CCENTERWND_H

#pragma once
#include <QWidget>
#include <map>
#include <vector>

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
    void slotRolesName(const QStringList roleNames);

signals:
    void signAddGptRole(const QString&  roleName,const QString&  sysTemPrompt,const QString&  temp);
    void signAskGpt(const QString& roleName,const QString& modelName,const QString& userPrompt);
    void signOaiReply(const QString& text);

    
private:   


    void menuBtnClicked();

    void setMenuWnd();
    void setGptWnd(); 
    void setEnglishWnd();
    void setVideoWnd();    
    void setTaskWnd();
    void setSettingWnd();
    void setUserWnd();


    QWidget * leftMenuWnd_{};
    QWidget * rightWnd_{};
    std::vector<CImgButton*> menuBtns_;
};

#endif