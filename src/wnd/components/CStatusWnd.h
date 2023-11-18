#ifndef CSTATUSWND_H
#define CSTATUSWND_H

#pragma once
#include <QFrame>

class QLabel;
class CStatusWnd:public QFrame
{
    Q_OBJECT
public:
    CStatusWnd(QWidget * parent = nullptr);
    ~CStatusWnd(){}
    void setLeftText(const QString & text);
    void setRightText(const QString & text);
    void setFont(const QFont & font);


private:
    QLabel * leftLabel_;
    QLabel * rightLabel_;

};

#endif