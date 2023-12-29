#pragma once

#include <QObject>
#include <QString>
#include <QImage>

struct UMessage
{
    size_t index;
    bool isSave;
    bool isShow;
    QString owner; 
    QString receiver;
    QString model;
    QString content;
    QImage image;
   
};
Q_DECLARE_METATYPE(UMessage)