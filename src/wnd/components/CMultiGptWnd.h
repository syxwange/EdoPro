
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
#include "../../core/UMessage.h"

class CMultiMessageWnd;

class CMultiGptWnd:public QSplitter
{
    Q_OBJECT
public:
    CMultiGptWnd(QWidget *parent = nullptr,Qt::Orientation orientation = Qt::Horizontal);
    ~CMultiGptWnd();

    void slotEditRole(const QString& name,QStringList roleInfo);
    void slotMultiOaiReply(QVector<UMesaage>&);
    void slotReset(const QStringList& rolesName);

signals:
    void signAddRole(const QString& name,const QString& systemPrompt,const QString& temperature,bool isAssistant);
    void signAskGpt(const QString& roleName,const QString& modelName,const QString& userPrompt);
    void signShowSaveMessage();
    void signMultiAskGpt(QVector<UMesaage>& messages);
    void signGetRoleInfo(QString roleName);
    void signDelRole(QString roleName);

private:
    void addModels(const QStringList& models);
    void addRoleBtn();
    void askBtnClicked();
    void createGptRole(const QString &text);
    void roleBtnClicked();
    void addFile();

    void modelBtnClicked();
    void createModelBtn(const QString &text);
    void messageWndClicked();

    void roleBtnDelClicked();
    void roleBtnEditClicked();


    QVBoxLayout *scrollLayout_;
    QVBoxLayout *gtpsLayout_;
    QHBoxLayout *modelsLayout_;
   
    QString roleName_{};
    QString modelName_{};
    
    std::vector<QPushButton*> roleBtns_;
    std::vector<QPushButton*> modelBtns_;
    QVector<CMultiMessageWnd*> messageWnds_;
    QPushButton* askBtn_{};
    QTextEdit* textEdit_{};
    bool isChatLive_ = false;
    QVector<QStringList> colors_{{"#fbffef","black","#02114d"},{"#fbffef","black","#02114d"},{"#fbffef","black","#02114d"}};
    QVector<UMesaage> messages_{};
    QMap<QString,QStringList> mapColors_{{"man",colors_[0]}};

};

