#pragma once

#include <QMainWindow>
#include "../core/UMessage.h"

class CGptWnd;
class CImgButton;
class CGpts;

class CMainWnd:public QMainWindow
{
    Q_OBJECT
public:

    CMainWnd(QWidget *parent = nullptr);
    ~CMainWnd(){}

    void createGptWnd(CGpts* gpts);
    void slotStatusLeftText(const QString& text);
    void slotStatusRightText(const QString& text);

signals:
    void signCreateGptWnd();  

private:
    void initUI();
    QWidget* createMenu();
    
    void menuClick(CImgButton* sendBtn);


public:
    CGptWnd* gptWnd_{};
private:
    QWidget * currentWnd_{};
    std::vector<CImgButton*> menuBtns_;
};