#pragma once
#include "Board.h"
#include <QVector>

Board::Board()
{
    clear();
}

void Board::setFlag(unsigned short y, unsigned short x)
{
    allBoard[y][x].setStateUp(2);
}

void Board::unSetFlag(unsigned short y, unsigned short x)
{
    allBoard[y][x].setStateUp(0);
}

void Board::clickAllMines()
{
    for (unsigned short int y=0 ; y < 15 ; y++)
    {
        for (unsigned short int x=0 ; x < 10 ; x++)
        {
            if (allBoard[y][x].isMine())
                {
                // if the mine is under the flag, we set the state 3 (we cleared it correctly)
                if (allBoard[y][x].getStateUp() == 2)
                        allBoard[y][x].setStateUp(3);
                    else    // otherwise, just click (that is, we pressed a mine)
                        allBoard[y][x].setStateUp(1);
                }
        }
    }
}

bool Board::checkFlag()
{
    uint8_t check = 0;
    for (unsigned short int y=0 ; y<15 ; y++)
    {
        for (unsigned short int x=0 ; x<10 ; x++)
        {
            if ((allBoard[y][x].getStateUp()==2))
                check++;
        }
    }
    return (check >= 15);
}

bool Board::isWin()
{
    bool win=true;
    for (unsigned short int y=0 ; y<15 ; y++)
    {
        for (unsigned short int x=0 ; x<10 ; x++)
        {   // 1) chose a flag, but there is no mine 2) did not choose a flag, but there is a mine
            if ((allBoard[y][x].getStateUp()==2 && !allBoard[y][x].isMine()) ||
                (allBoard[y][x].getStateUp()!=2 && allBoard[y][x].isMine()))
                win=false;
        }
    }
    return win;
}

// if we hit an empty field, it works on recursion
void Board::clickNeigbors(unsigned short y, unsigned short x)
{
    if (y>0 && x>0)
        if (!allBoard[y-1][x-1].isClicked())
    {
         // we are setting state 1
        allBoard[y-1][x-1].setStateUp(1);
        if (allBoard[y-1][x-1].getStateDown()==0)
            clickNeigbors(y-1,x-1);
    }

    if (y>0)         if (!allBoard[y-1][x  ].isClicked()){ allBoard[y-1][x  ].setStateUp(1);if (allBoard[y-1][x  ].getStateDown()==0) clickNeigbors(y-1,x  );}
    if (y>0 && x<9)  if (!allBoard[y-1][x+1].isClicked()){ allBoard[y-1][x+1].setStateUp(1);if (allBoard[y-1][x+1].getStateDown()==0) clickNeigbors(y-1,x+1);}

    if (x>0)         if (!allBoard[y  ][x-1].isClicked()){ allBoard[y  ][x-1].setStateUp(1);if (allBoard[y  ][x-1].getStateDown()==0) clickNeigbors(y  ,x-1);}
    if (x<9)         if (!allBoard[y  ][x+1].isClicked()){ allBoard[y  ][x+1].setStateUp(1);if (allBoard[y  ][x+1].getStateDown()==0) clickNeigbors(y  ,x+1);}

    if (y<13&& x>0)  if (!allBoard[y+1][x-1].isClicked()){ allBoard[y+1][x-1].setStateUp(1);if (allBoard[y+1][x-1].getStateDown()==0) clickNeigbors(y+1,x-1);}
    if (y<13)        if (!allBoard[y+1][x  ].isClicked()){ allBoard[y+1][x  ].setStateUp(1);if (allBoard[y+1][x  ].getStateDown()==0) clickNeigbors(y+1,x  );}
    if (y<13&& x<9)  if (!allBoard[y+1][x+1].isClicked()){ allBoard[y+1][x+1].setStateUp(1);if (allBoard[y+1][x+1].getStateDown()==0) clickNeigbors(y+1,x+1);}
}

void Board::setMineBoom(unsigned short y, unsigned short x)
{
    allBoard[y][x].setStateDown(10);
}

QString Board::getColorNumber(unsigned short int y, unsigned short int x)
{
    switch (allBoard[y][x].getStateDown()) {
        case  1 : { return "#4040ff";break;}
        case  2 : { return "#66cc00";break;}
        case  3 : { return "#ff6347";break;}
        case  4 : { return "#08007e";break;}
        case  5 : { return "#3527ff";break;}
        case  6 : { return "#ff1717";break;}
        case  7 : { return "#990000";break;}
        case  8 : { return "#2d4a08";break;}
        default : { return "#452828";break;}
    }
}

void Board::clear()
{
    for (unsigned short int y=0 ; y<15 ; y++)
    {
        for (unsigned short int x=0 ; x<10 ; x++)
        {
            allBoard[y][x].setStateUp(0);
            allBoard[y][x].setStateDown(0);
        }
    }
}

void Board::generateNew()
{
    struct Wsp
    {
        unsigned short int y=0;
        unsigned short int x=0;
    };
    Wsp temp;
    bool isTemp;
    QVector<Wsp> mineCounter;
    uint8_t bomb = 15;
    while (mineCounter.size()<bomb)
    {
        temp.y = rand()%15;
        temp.x = rand()%10;
        isTemp=false;
        for (auto iter=mineCounter.begin() ; iter != mineCounter.end() ; iter++)
        {
            // if bombs are already planted at these coordinates
            if (iter->x==temp.x && iter->y==temp.y)
            {
                isTemp = true;
                break;
            }//if
        }//for

        if (!isTemp)
        {
            mineCounter.push_back(temp);
              // for the variable, we set the bomb in coordinates
            allBoard[temp.y][temp.x].setMine();
        }//if

    }//while

    for (unsigned short int y=0; y<15 ; y++)
    {
        for (unsigned short int x=0; x<10; x++)
        {
            if (!allBoard[y][x].isMine())
            {
                // setStateDown is implemented in myCell, it determines how many bombs around the bomb 1 - 8
                allBoard[y][x].setStateDown(numberOfMin(y,x));

            } // if 1
        } //for x
    } // for y

}

Call_states Board::get(unsigned short int y, unsigned short int x)
{
    return allBoard[y][x];
}

void Board::setClick(unsigned short int y, unsigned short int x)
{
    allBoard[y][x].setStateUp(1);
}

bool Board::isMine(unsigned short int y, unsigned short int x)
{
   if (allBoard[y][x].isMine())
       return true;
   else
       return false;
}

unsigned short int Board::numberOfMin(unsigned short int y, unsigned short int x)
{
    unsigned short int temp=0;
    if (y>0 && x>0) if (allBoard[y-1][x-1].isMine())    temp++;
    if (y>0)        if (allBoard[y-1][x].isMine())      temp++;
    if (y>0 && x<9) if (allBoard[y-1][x+1].isMine())    temp++;

    if (x>0)        if (allBoard[y][x-1].isMine())      temp++;
    if (x<9)        if (allBoard[y][x+1].isMine())      temp++;

    if (y<13&& x>0) if (allBoard[y+1][x-1].isMine())    temp++;
    if (y<13)       if (allBoard[y+1][x].isMine())      temp++;
    if (y<13&& x<9) if (allBoard[y+1][x+1].isMine())    temp++;
    return temp;
}
