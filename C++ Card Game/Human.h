    /************************************************************
     * Name:  Adam Roberts                                      *
     * Project:  Project 1 / GO STOP C++                        *
     * Class:  CMPS 366 01                                      *
     * Date:  February 18, 2020                                 *
     ************************************************************/

#ifndef HUMAN_H
#define HUMAN_H
#include "Player.h"

using namespace std;

class Human : public Player
{
    private:

        void move();
        void move2a();
        void move2b();
        void printHand();
        void printPairs();
        string suggestedMove();
        
    public:

        Human();
        Human(int flag);


};
#endif