    /************************************************************
     * Name:  Adam Roberts                                      *
     * Project:  Project 1 / GO STOP C++                        *
     * Class:  CMPS 366 01                                      *
     * Date:  February 18, 2020                                 *
     ************************************************************/
    
#ifndef COMPUTER_H
#define COMPUTER_H
#include "Dealer.h"

using namespace std;

class Computer : public Player
{
    private:

        void move();
        void move2a();
        void move2b();
        void printHand();
        string suggestedMove();
        

    public:
        Computer();
        Computer(int flag);


};
#endif
