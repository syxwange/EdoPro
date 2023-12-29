
#pragma once

#include <QSplitter>
#include <QKeyEvent>
#include <QTextEdit>
#include "../../core/UMessage.h"
#include <QImage>

class CMsgWnd;
class QPushButton;
class CMessageWnd;
class QVBoxLayout;
class QHBoxLayout;
class CImgButton;
class QTextEdit;
class QScrollArea;
class CGpts;

class CGptWnd:public QSplitter
{
    Q_OBJECT
public:
    CGptWnd(CGpts* gpts,QWidget *parent = nullptr,Qt::Orientation orientation = Qt::Horizontal);
    ~CGptWnd(){}   
    void slotGptReply(const UMessage& msg);    

signals:
    void signAskGpt(UMessage& msg);  

    

private: 
    void init();
    
    void askBtnClick();

    void msgWndSaveClick(size_t id,bool isSave);
    void msgWndcloseClick(size_t id);
    void showMessage(bool isShow=false);
    void saveMessage();
    void addFile();
    void openfile();

    void modelBtnClick();
    void createModelBtn(const QString &text);

    void roleBtnClick();   
    void roleBtnDelClick();
    void roleBtnEditClick();
    void createRoleBtn(const QString &text);    
    void addRole(const QString & name={},const std::map<QString,QString>& info={},bool isEdit=false);
    


    QVBoxLayout *msgWndLaout_;
    QVBoxLayout *gtpsLayout_;
    QHBoxLayout *modelsLayout_;
    QScrollArea * scrollArea_;
   
    QString roleName_{};
    QString modelName_{};
    
    std::vector<QPushButton*> roleBtns_;
    std::vector<QPushButton*> modelBtns_;
    std::vector<CMsgWnd*> msgWnds_;
 
    QPushButton* askBtn_{};
    QTextEdit* textEdit_{}; 
    QImage image_{};
    QVector<QStringList> colors_{{"#F3FFEE","black","#02114d"},{"#eaf6fd","#0b0121","#02114d"},{"#fffaf3","#160d00","#02114d"},{"#f5eefd","#181c25","#02114d"},{"#eef0fe","#020830","#02114d"}};

    QMap<QString,QStringList> mapColors_{{"man",{"#fbffef","black","#02114d"}}};
    CGpts* gpts_{};
};

