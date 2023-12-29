#include "CScreenCapture.h"
#include <QPixmap>
#include <QMouseEvent>
#include <QScreen>
#include <QGuiApplication>
#include <tesseract/baseapi.h>

CScreenCapture::CScreenCapture(QWidget *parent): QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}


void CScreenCapture::mousePressEvent(QMouseEvent *event)  {
    if (event->button() == Qt::LeftButton) {
        m_selecting = true;
        m_startPos = event->pos();
        m_endPos = m_startPos;
        update();
    }
}

void CScreenCapture::mouseMoveEvent(QMouseEvent *event)  {
    if (m_selecting) {
        m_endPos = event->pos();
        update();
    }
}

void CScreenCapture::mouseReleaseEvent(QMouseEvent *event)  {
    if (event->button() == Qt::LeftButton && m_selecting) {
        m_selecting = false;
        // 这里可以根据选取的区域进行相应的处理
        // 例如，绘制矩形阴影或执行其他操作
        update();
        QRect selectRect = QRect(m_startPos, m_endPos).normalized();
        QScreen *screen = QGuiApplication::primaryScreen();
        QPixmap screenshot = screen->grabWindow(0,selectRect.x(),selectRect.y(),selectRect.width(),selectRect.height());
         
        QString text = img2text(screenshot);
        emit signPictureText(text);
        hide();
    }
}

void CScreenCapture::paintEvent(QPaintEvent *event)  {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor(0, 0, 0, 5)); // 绘制半透明背景

    if (m_selecting) {
        QRect selectRect = QRect(m_startPos, m_endPos).normalized();
        // painter.setPen(Qt::red);
        painter.setPen(QColor(125,125,125));
        painter.drawRect(selectRect); // 绘制选取的矩形
        // painter.fillRect(selectRect, QColor(0, 0, 0, 64)); // 绘制半透明背景
    }
}

QString CScreenCapture::img2text(QPixmap pixmap)
{
    tesseract::TessBaseAPI* ocr = new tesseract::TessBaseAPI();  
    // 初始化OCR引擎
    const char * dataPath = "D:/wg/wgDataLib/data/tesseractOcrData";
    if (ocr->Init(dataPath, "eng+chi_sim")) {
        qDebug() << "无法初始化OCR引擎";
        return {};
    }
    // 加载图像
    QImage image = pixmap.toImage();
    // 设置要识别的语言为中文和英文
    ocr->SetVariable("tessedit_languages", "eng+chi_sim");
    // 设置图像
    ocr->SetImage(image.bits(), image.width(), image.height(), 4, image.bytesPerLine());
    // 进行文本识别
    char* cText = ocr->GetUTF8Text();
    //把不正常的换行符替换成空格
    QString text =  QString::fromUtf8(cText);
    int index = text.indexOf('\n');
    while (index != -1 && index > 0) {
        QChar prevChar = text.mid(index - 1, 1)[0];
        if (!(prevChar == '.' || prevChar == '?' || prevChar == '!'))
        {
            text.replace(index, 1, ' ');
        }
        index = text.indexOf('\n', index + 1);
    }  
    return text;
}
