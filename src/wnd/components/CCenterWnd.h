
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

private:   

    void init();

    void createGptWnd(const QString& name);
    void gptsBtnClicked();
    void videoBtnClicked();
    void englishBtnClicked();
    void taskBtnClicked();

    void btnClicked(CImgButton* sendBtn);

    QHBoxLayout * hLayout_{};
    QWidget * leftMenuWnd_{};
    QWidget * rightWnd_{};
    std::vector<CImgButton*> menuBtns_;
};

