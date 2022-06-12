#pragma once
#include "Sapper.h"
#include <QMessageBox>
#include "Click_button.h"
#include "main.cpp"

Sapper::Sapper(QWidget* pwgt/* = 0*/) : QWidget(pwgt)
{
    Game_over = false;
    createBoard();
    Sapper_Board.generateNew();
    showBoard();

}

void Sapper::createBoard()
{
    QFrame* pfrm = new QFrame;
    pfrm->setFrameStyle(QFrame::WinPanel|QFrame::Plain);
    pfrm->setLineWidth(2);
    ptopLayout = new QGridLayout;
    //////////////////////////////////////////

    QPushButton* pcmdA = new QPushButton("Restart");
    QPushButton* pcmdB = new QPushButton("Exit");
    pcmdA->setStyleSheet("background-color: #ffa500");
    pcmdB->setStyleSheet("background-color: #ffa500;");
    setStyleSheet("background-color:#ffdead");

    ptopLayout->addWidget(pcmdA,0,0,1,6);
    ptopLayout->addWidget(pcmdB,0,6,1,5);

    connect(pcmdA,SIGNAL(clicked()),this,SLOT(New_Game_Click()));
    connect(pcmdB,SIGNAL(clicked()),this,SLOT(close()));
    //////////////////////////////////////////

    // Шрифт чисел внутри кнопок
    QFont ButtonFont;
    ButtonFont.setPixelSize(14);
    ButtonFont.setFamily("Times New Roman");
    ButtonFont.setBold(1);

    for (uint8_t y=0 ; y<15 ; y++)
    {
        for (uint8_t x=0 ; x<10 ; x++)
        {
            // creating a button and
            // we give the variable a name = (ordinal number),
            //by which we can find the button

            Visual_Button[y][x] = new Click_Button();
            Visual_Button[y][x]->setObjectName(QVariant(y*10+x).toString());

            Visual_Button[y][x]->setFont(ButtonFont);
            Visual_Button[y][x]->setIconSize(QSize(20, 20));
            Visual_Button[y][x]->setFixedSize(25,25);

            ptopLayout->addWidget(Visual_Button[y][x],y+1,x+1);

            // connecting the buttons. The signal is pressed the left mouse button connects to the slot
            // connect(a pointer to an object; a signal with which we are connecting;
            // pointer to an object that has a processing slot; the slot that is called when a signal is received)

            connect(Visual_Button[y][x],SIGNAL(leftClicked() ), this, SLOT(Button_Click()));
            connect(Visual_Button[y][x],SIGNAL(rightClicked()), this,SLOT(Button_Click_Right()));
        }// for x
    }//for y

    ptopLayout->addWidget(pfrm);
    ptopLayout->setSpacing(2);

    setLayout(ptopLayout);
}

/*  Saper_Board.generateNew();  */

void Sapper::showBoard()
{
    for (uint8_t y=0 ; y< 15 ; y++)
    {
    for (uint8_t x=0 ; x< 10 ; x++)
    {
        if (Sapper_Board.get(y,x).isClicked())
            {
            Visual_Button[y][x]->setFlat(1);
            if (Sapper_Board.get(y,x).isMine())
                {
                // Explosion?
                if (Sapper_Board.get(y,x).getStateDown() == 10)
                    {
                        Visual_Button[y][x]->setIcon(QIcon(":/picture/img/02mine.png"));
                    }
                    else
                    {  // No, why?
                        // because we cleared it? (3 - flag is set)
                        if (Sapper_Board.get(y,x).getStateUp() == 3)
                        {
                            Visual_Button[y][x]->setIcon(QIcon(":/picture/img/05mine.png"));
                        }// No, showing bombs to the loser
                        else
                        {
                            Visual_Button[y][x]->setIcon(QIcon(":/picture/img/01mine.png"));
                        }//if 4
                    }//if 3
                }//if2

            if (Sapper_Board.get(y,x).getStateDown()>0 && Sapper_Board.get(y,x).getStateDown()<9)
                {
                    Visual_Button[y][x]->setText(QVariant(Sapper_Board.get(y,x).getStateDown()).toString());
                    Visual_Button[y][x]->setStyleSheet("color: " + Sapper_Board.getColorNumber(y,x) + ";");
                }
        }// if 1
    else
        {
            Visual_Button[y][x]->setFlat(0);
            Visual_Button[y][x]->setText("");
            Visual_Button[y][x]->setStyleSheet("background-color: #f4a460");

            if (Sapper_Board.get(y,x).isFlag())
            {
            // is the game over? and the button did not turn out to be a mine,
            // then we write that the checkbox is located by mistake
                if (Game_over && Sapper_Board.get(y,x).isMine()==false)
                    Visual_Button[y][x]->setIcon(QIcon(":/picture/img/06flag.png"));
                else    // otherwise everything is correct and we continue to play
                    Visual_Button[y][x]->setIcon(QIcon(":/picture/img/03flag.png"));
            }// if 2
            else    // if it's just a button, we output a void
            {
                Visual_Button[y][x]->setIcon(QIcon(""));
            }
        }//if 1
    }// for x
    }//for y
}

unsigned short int Sapper::getNumberButtonX(unsigned short int numberObjectButton)
{
    unsigned short int temp = numberObjectButton / 10;
    return numberObjectButton - 10 * temp;
}

unsigned short int Sapper::getNumberButtonY(unsigned short int numberObjectButton)
{
    return numberObjectButton / 10;
}

void Sapper::Button_Click()
{
    if (!Game_over)
        {
            unsigned short int y = getNumberButtonY(QVariant(((QPushButton*)sender())->objectName()).toInt());
            unsigned short int x = getNumberButtonX(QVariant(((QPushButton*)sender())->objectName()).toInt());

            Sapper_Board.setClick(y,x);

            // we ask how many bombs are on the cell, if there are 0 of them
            if (Sapper_Board.get(y,x).getStateDown()==0)
            {
                Sapper_Board.clickNeigbors(y,x);
            }
            else
            {
                if (Sapper_Board.isMine(y,x))
                {
                    Game_over = true;
                    Sapper_Board.clickAllMines();
                    Sapper_Board.setMineBoom(y,x);
                    QMessageBox::information(this, "BAAAAM", "You lose!");
                }
            }
            showBoard();
        }
}

void Sapper::Button_Click_Right()
{
    if (!Game_over)
        {
            unsigned short int y = getNumberButtonY(QVariant(((QPushButton*)sender())->objectName()).toInt());
            unsigned short int x = getNumberButtonX(QVariant(((QPushButton*)sender())->objectName()).toInt());

            if (!Sapper_Board.get(y,x).isClicked())
                {
                    if (Sapper_Board.get(y,x).isFlag())
                        {
                            Sapper_Board.unSetFlag(y,x);
                        }
                    else
                        {
                            if(!Sapper_Board.checkFlag())
                            {
                                Sapper_Board.setFlag(y,x);
                                if (Sapper_Board.isWin())
                                {
                                    Game_over=true;
                                    Sapper_Board.clickAllMines();
                                    QMessageBox::information(this, "Fuuuuuh", "You win!");
                                }//if 4
                            }//if 3
                        }// if 2
                }// if 1
            showBoard();
        }
}

void Sapper::New_Game_Click()
{
    Game_over = false;
    Sapper_Board.clear();
    Sapper_Board.generateNew();
    showBoard();
}
