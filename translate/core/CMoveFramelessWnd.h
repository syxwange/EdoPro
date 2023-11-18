#pragma once


#include <QWidget>


class CMoveFramelessWnd : public QWidget
{
    Q_OBJECT
public:
    explicit CMoveFramelessWnd(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event)override;
    void mouseMoveEvent(QMouseEvent *event)override;

private:
    QPointF movePoint_; //移动的距离
    bool bMousePress_; //按下鼠标左键
    QPoint startPos_;
    QRect originalGeometry_;
    Qt::Edges resizeFlags_{};
    bool isMove_{true};
};


