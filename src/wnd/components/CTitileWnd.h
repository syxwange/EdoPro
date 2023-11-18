#ifndef CTITILEWND_H
#define CTITILEWND_H

#pragma once
#include <vector>
#include <QWidget>
#include <QFrame>

class QLabel;
class CTitileWnd:public QFrame
{
    Q_OBJECT
public:
    CTitileWnd(QWidget *parent = nullptr);
    ~CTitileWnd(){}

    void setTitle(const QString & title,const QFont& font = QFont("Microsoft YaHei", 12));
    void setIcon(const QString & icon,const QSize &size = QSize(30,30));

signals:
    void signCloseWnd();
    void signMinWnd();

private:
    QLabel * imgLabel_;
    QLabel * titleLabel_;
};

#endif