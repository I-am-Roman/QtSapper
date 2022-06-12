#pragma once

class Call_states
{

private:
    // states hidden under the button
    // 0 - 8 - the numbers of the number of mines, 9 - bomb, 10 - bomb exploded
    unsigned short int stateDown;
    // 0 - by itself, 1 - it was clicked, 2 - the flag was set, 3 - it was clicked and the flag was set
    unsigned short int stateUp;

public:
    // constructor, we make all states 0
    Call_states();
    // get the status under the button
    unsigned short int getStateDown();
    // get the status on the button
    unsigned short int getStateUp();

    // set the status under the button
    void setStateDown(unsigned short int);
    // set the status on the button
    void setStateUp(unsigned short int);

    bool isMine();
    bool isFlag();
    bool isClicked();
    void setMine();
};

