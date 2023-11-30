
#pragma once

class QPushButton;
class CMessageWnd;
class QVBoxLayout;
class QHBoxLayout;
class CImgButton;
class QTextEdit;
class QScrollArea;

#include <QSplitter>
#include <QKeyEvent>
#include <QTextEdit>
#include "../../core/UMessage.h"

class CMultiMessageWnd;

class CMultiGptWnd:public QSplitter
{
    Q_OBJECT
public:
    CMultiGptWnd(QWidget *parent = nullptr,Qt::Orientation orientation = Qt::Horizontal);
    ~CMultiGptWnd();

    void slotMultiOaiReply();
    void initRoleBtn(const QString& );

private:
    void addModels(const QStringList& models);
    void addRole(const QString & name={},const QStringList& info={},bool isEdit=false);
    void askBtnClicked();
    void createRoleBtn(const QString &text);
    void createModelBtn(const QString &text);
    void saveMessage();
    void addFile();
    void showMessage();

    void modelBtnClicked();
    void roleBtnClicked();    
    void messageWndClicked();
    void roleBtnDelClicked();
    void roleBtnEditClicked();


    QVBoxLayout *scrollLayout_;
    QVBoxLayout *gtpsLayout_;
    QHBoxLayout *modelsLayout_;
    QScrollArea * scrollArea_;
   
    QString roleName_{};
    QString modelName_{};
    
    std::vector<QPushButton*> roleBtns_;
    std::vector<QPushButton*> modelBtns_;
    QVector<CMultiMessageWnd*> messageWnds_;
    QPushButton* askBtn_{};
    QTextEdit* textEdit_{}; 
    QVector<QStringList> colors_{{"#F3FFEE","black","#02114d"},{"#eaf6fd","#0b0121","#02114d"},{"#fffaf3","#160d00","#02114d"},{"#f5eefd","#181c25","#02114d"},{"#eef0fe","#020830","#02114d"}};

    QMap<QString,QStringList> mapColors_{{"man",{"#fbffef","black","#02114d"}}};

};

