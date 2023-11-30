#ifndef CEDUPRO_H
#define CEDUPRO_H

#pragma once
#include <QObject>
#include "openai/COpenAI.h"
#include <vector>
#include "core/UMessage.h"

class CMainWnd;
class CScreenCapture;
class COpenAI;


class CEduPro:public QObject
{
    Q_OBJECT
public:
    static CEduPro * app(){static CEduPro app;return &app;}

    ~CEduPro(){}
    void show();
    /**
     * @brief 槽函数，用于处理 OAI 回复的信号。
     * 
     * @param text 回复的文本内容。
     * @param statusCode 回复的状态码。
     */
    void slotOaiReply(const QString& text,int statusCode);

    /**
     * @brief 槽函数，从配置文件config.ini中获取创建name窗口的数据。
     * 
     * @param name 窗口的名称。
     */
    QStringList getDataForWnd(const QString& name);

    /**
     * @brief Handles the slot for asking GPT.
     * 
     * This function is responsible for handling the slot for asking GPT.
     * It takes three parameters: roleName, modelName, and userPrompt.
     * 
     * @param roleName The role name.
     * @param modelName The model name.
     * @param userPrompt The user prompt.
     */
    void slotAskGpt(const QString& roleName,const QString& modelName,const QString& userPrompt);
    
    /**
     * @brief 添加角色的槽函数
     * 
     * @param name 角色名称
     * @param systemPrompt 系统提示词
     * @param temperature 温度
     * @param isAssistant 是否是助手
     */
    void saveRole(const QString& name,const QString& systemPrompt,const QString& temperature);

    void multiAskGpt();  

    void delRole(const QString& name);

    void saveMsgAsJson(const QString& fileName="autoSave.json");

    /// @brief 
    /// @param fileName 
    void loadMsgFromJson(const QString& fileName="autoSave.json");

signals:   
    void signMultiOaiReply();
    void signStatusText(const QString& text);
    void signOaiReply(const QString& text);
    void signStatusRightText(const QString& text);
    
public:
    QVector<UMesaage> messages_{};
    std::map<QString, QStringList> rolesInfo_{};
    QString currentTheme_{"gpts"}; 

private:

    CEduPro();
    /**
     * @brief 创建OIA消息
     * 
     * @param roleName 角色名称
     * @param modelName 模型名称
     * @param userPrompt 用户提示
     * @return QByteArray 返回OIA消息的字节数组
     */
    QByteArray createOiaMessage(const QString& roleName,const QString& modelName,const QString& userPrompt);
    
    void connectSignals();
    
    void init() ;

    CMainWnd * win_;
    COpenAI * openai_;    
    std::map<QString,QString> modelsName_{}; 
    QString currentRole_{};
    float money_{0}; 
};

#endif