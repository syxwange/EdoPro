#ifndef CEDUPRO_H
#define CEDUPRO_H

#pragma once
#include "openai/COpenAI.h"


class CScreenCapture;
class QVBoxLayout;
class CMessageWnd;
#include "core/CMoveFramelessWnd.h"
#include <vector>

class CTranslate:public CMoveFramelessWnd
{
    Q_OBJECT
public:
    CTranslate(QWidget *parent = Q_NULLPTR);
    ~CTranslate();
    void slotPictureText(const QString& text);
    void activeScreen();
    void slotOaiReply(const QString& text);
signals:    

protected:
    void changeEvent(QEvent *event) override;
    void closeEvent(QCloseEvent* event) override;


private:
    void setSystray();
    COpenAI * openai_;
    CScreenCapture *pScreenCapture_;
    bool isSHide_{false};
    QVBoxLayout *layout_;
    std::vector<CMessageWnd*> messageWnds_;

};

#endif