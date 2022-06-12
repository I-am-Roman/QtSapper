#pragma once

#include <QPushButton>
#include <QWidget>
#include <QGridLayout>
#include "Click_button.h"
#include "Board.h"

class Sapper : public QWidget
{
    Q_OBJECT

private:
    // own variable type
    // I create a special field for the user 15x10
    Click_Button* Visual_Button[15][10];
    // if the game is over? (true) - the game is blocked
    bool Game_over;
    unsigned short int getNumberButtonX(unsigned short int numberObjectButton);
    unsigned short int getNumberButtonY(unsigned short int numberObjectButton);

public:
    Board Sapper_Board;
    Sapper(QWidget* pwgt = 0);

    void createBoard();

    void showBoard();

    QGridLayout* ptopLayout;

public slots:
    // Left mouse click
    void Button_Click();
    void Button_Click_Right();
    void New_Game_Click();
};
