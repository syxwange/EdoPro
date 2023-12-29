#pragma once

#include <QPushButton>
#include <QEvent>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>

class CImgButton : public QPushButton
{
    Q_OBJECT
public:
    //
    CImgButton(const QString &normalImagePath, const QString &hoverImagePath, QSize size = QSize(24, 24), QWidget *parent = nullptr)
    : QPushButton(parent), normalImage(normalImagePath), hoverImage(hoverImagePath)
    {
        setStyleSheet("QPushButton{border: none;}"); // 去掉按钮的边框
        setIcon(QIcon(normalImagePath));
        setIconSize(size);
        connect(this, &QPushButton::clicked, [this](){isClicked_ = true;setIcon(QIcon(hoverImage));});
    }

    void slotSetUnClicked()
    {
        isClicked_ = false;
        setIcon(QIcon(normalImage));
    }

protected:


    void enterEvent(QEnterEvent *event) override
    {
        QPushButton::enterEvent(event);
        setIcon(QIcon(hoverImage));
    }

    void leaveEvent(QEvent *event) override
    {
        QPushButton::leaveEvent(event);
        if (!isClicked_)
            setIcon(QIcon(normalImage));
    }


private:
    QString normalImage;
    QString hoverImage;
    bool isClicked_ = false;
};



class CMenuButton : public QPushButton {
    Q_OBJECT

public:
    CMenuButton(const QString& text,QWidget * parent = nullptr) : QPushButton(text,parent) {}

protected:
    void contextMenuEvent(QContextMenuEvent* event) override {
        QMenu menu(this);

        QAction* action1 = new QAction("edit", this);
        connect(action1, &QAction::triggered, this, &CMenuButton::signEditClicked);
        menu.addAction(action1);

        QAction* action2 = new QAction("del", this);
        connect(action2, &QAction::triggered, this, &CMenuButton::signDelClicked);
        menu.addAction(action2);
        menu.setStyleSheet("QMenu::item:selected { background-color: #A0A0A0;}");
        menu.exec(event->globalPos());   
    }

signals:
    void signEditClicked();
    void signDelClicked();

};
