#include "CMoveFramelessWnd.h"
#include <QPainter>
#include <QMouseEvent>
#include <qmath.h>
#include <QPainterPath>

CMoveFramelessWnd::CMoveFramelessWnd(QWidget *parent) :   QWidget(parent), bMousePress_(false)
{
    setWindowFlags(Qt::FramelessWindowHint );
    this->setObjectName("basewidget");
    setMouseTracking(true);
    // setAttribute(Qt::WA_TranslucentBackground);
}

void CMoveFramelessWnd::mousePressEvent(QMouseEvent *event)
{
    //只能是鼠标左键移动和改变大小
    if(event->button() == Qt::LeftButton)
    {
        movePoint_ = event->globalPosition() - pos();
        startPos_ = event->globalPosition().toPoint();
        bMousePress_ = true;  
        originalGeometry_= geometry();
        isMove_=true;
        const int border = 8; // 边框宽度
        auto pos = event->pos();       
        if (pos.x()<border){ 
            resizeFlags_ = Qt::LeftEdge;
            isMove_ = false;
        }
       
        if (pos.x() > width() - border) {
            resizeFlags_ = Qt::RightEdge;
            isMove_ = false;
        }
       
        if (pos.y()< border ){
            resizeFlags_ = Qt::TopEdge;
            isMove_ = false;
        }
        
        if (pos.y() > height() - border){ 
            resizeFlags_ = Qt::BottomEdge;
            isMove_ = false;
        }  
    }     
    
}

void CMoveFramelessWnd::mouseReleaseEvent(QMouseEvent *)
{
    bMousePress_ = false;
    resizeFlags_=resizeFlags_&~resizeFlags_;
}

void CMoveFramelessWnd::mouseMoveEvent(QMouseEvent *event)
{    
    if(bMousePress_)
    {
        QPointF movePpos = event->globalPosition();  
        QPoint delta = movePpos.toPoint() - startPos_; 
        QRect newGeometry = originalGeometry_;

        if (resizeFlags_ & Qt::LeftEdge ) {
            newGeometry.setLeft(originalGeometry_.left() + delta.x());
            setGeometry(newGeometry);
        }
        if (resizeFlags_ & Qt::RightEdge) {
            newGeometry.setRight(originalGeometry_.right() + delta.x());
            setGeometry(newGeometry);
        }
        if (resizeFlags_ & Qt::TopEdge) {
            newGeometry.setTop(originalGeometry_.top() + delta.y());
            setGeometry(newGeometry);
        }
        if (resizeFlags_ & Qt::BottomEdge) {
            newGeometry.setBottom(originalGeometry_.bottom() + delta.y());
            setGeometry(newGeometry);            
        } 
        //移动窗口       
        if (isMove_){
            move(movePpos.toPoint() - movePoint_.toPoint());           
        }
   
    }
    
 
    QPoint cursorPos = event->pos();
    // 获取窗口大小
    QSize size = this->size();
    // 设置边框的触发宽度
    const int borderWidth = 8;
    // 判断鼠标是否在边框上
    if (cursorPos.x() < borderWidth || cursorPos.x() > width() - borderWidth)
        setCursor(Qt::SizeHorCursor);
    else if (cursorPos.y() < borderWidth || cursorPos.y() > height() - borderWidth)
        setCursor(Qt::SizeVerCursor); 
    else
        unsetCursor(); 
}
