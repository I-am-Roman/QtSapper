#pragma once

#include <QPushButton>
#include <QMouseEvent>

// my own class for buttons
class  Click_Button : public QPushButton
{
    Q_OBJECT

public:
    explicit  Click_Button(QWidget* parent = 0);

public slots:
    void mousePressEvent(QMouseEvent* e);

signals:
    void rightClicked();
    void leftClicked();
};

