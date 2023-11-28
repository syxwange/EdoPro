#pragma once

#include <QObject>
#include <QString>

struct UMesaage
{
    size_t index;
    bool isSave;
    bool isShow;
    QString owner; 
    QString receiver;
    QString model;
    QString content;
   
};
Q_DECLARE_METATYPE(UMesaage)