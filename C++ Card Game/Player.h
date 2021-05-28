    /************************************************************
     * Name:  Adam Roberts                                      *
     * Project:  Project 1 / GO STOP C++                        *
     * Class:  CMPS 366 01                                      *
     * Date:  February 18, 2020                                 *
     ************************************************************/

#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <string>

using namespace std;

class Player
{
    private:

    protected:        
        vector<string> hand;
        vector<char> pairs;
        double matchingPair;  
        int points;              
        string userName;

        virtual void move2a(){};
        virtual void move2b(){};
        
    public:

        Player();

        void addCard(string card);
        void addPoints();
        void addPair(char card);  
        void drawCard();
        string getCard(int loc); 
        string getName();        
        char getPair(int loc);
        int getPoints();
        void handClear();
        int handSize();
        virtual void move(){};
        void pairsClear();
        int pairSize(); 
        virtual void printHand(){};
        virtual void printPairs(){};
        void setPoints(int point);
        virtual string suggestedMove(){return "";}; 

};
#endif