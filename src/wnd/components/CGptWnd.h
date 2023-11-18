#ifndef CGPTWND_H
#define CGPTWND_H

#pragma once

class QPushButton;
class CMessageWnd;
class QVBoxLayout;
class QHBoxLayout;
class CImgButton;
class QTextEdit;

#include <QSplitter>
#include <QKeyEvent>
#include <QTextEdit>

class CGptWnd:public QSplitter
{
    Q_OBJECT
public:
    CGptWnd(QWidget *parent = nullptr,Qt::Orientation orientation = Qt::Horizontal);
    ~CGptWnd();

    void slotRolesName(const QStringList& rolesName);     
    void slotOaiReply(const QString &text);

signals:
    void signAddGptRole(const QString& roleName,const QString& sysTemPrompt,const QString& temp);
    void signAskGpt(const QString& roleName,const QString& modelName,const QString& userPrompt);

private:
    void addModels(const QStringList& models);
    void addRoleBtn();
    void askBtnClicked();
    void createGptRole(const QString &text);
    void roleBtnClicked();
    void addFile();

    void modelBtnClicked();
    void createModelBtn(const QString &text);
  

    QVBoxLayout *scrollLayout_;
    QVBoxLayout *gtpsLayout_;
    QHBoxLayout *modelsLayout_;
   
    QString roleName_{};
    QString modelName_{};
    
    std::vector<QPushButton*> roleBtns_;
    std::vector<QPushButton*> modelBtns_;
    std::vector<CMessageWnd*> messageWnds_;
    QPushButton* askBtn_{};
    QTextEdit* textEdit_;
    bool isChatLive_ = false;

};

#endif