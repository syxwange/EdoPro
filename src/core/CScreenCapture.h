#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>

class CScreenCapture: public QWidget 
{
    Q_OBJECT
public:
    CScreenCapture(QWidget *parent = nullptr); 
    ~CScreenCapture(){}

signals:
    void signPictureText(const QString& text);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override ;
    void mouseReleaseEvent(QMouseEvent *event) override ;
    void paintEvent(QPaintEvent *event) override;

private:
    QString img2text(QPixmap pixmap);

    bool m_selecting{false};
    QPoint m_startPos{};
    QPoint m_endPos{};
};

