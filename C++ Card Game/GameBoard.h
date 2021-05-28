    /************************************************************
     * Name:  Adam Roberts                                      *
     * Project:  Project 1 / GO STOP C++                        *
     * Class:  CMPS 366 01                                      *
     * Date:  February 18, 2020                                 *
     ************************************************************/

#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include "Deck.h"
#include "Dealer.h"
#include "Player.h"

using namespace std;

class GameBoard
{
   

    public:
    
        private:
        
            enum gameMode {load, game, roundRobin, debug};
            static gameMode gameType;
            Dealer* Ethel; 
            static vector<Player*> players; 
            static vector<vector<string>> layout;                       
            static double round;
            static string playersTurn;
            static bool debugMode;   
            bool gameLoaded = false;
            bool rounds;

            void determinDebug();
            void determinGameMode();
            void displayBoard();
            void endGame();  
            void loadGame();                         
            void playGame();
            void playRounds();
            void printlayout();            
            void removeSubstrs(string& s, string p);         
            void startDebug();            

        public:         

            GameBoard();
            static void saveGame();
            static void addLayoutEnd(string card);
            static void addLayoutStack(int col, string card);
            static void drawLayout();
            static void eraseStack(int col); 
            static bool getDebug();
            static gameMode getGameType();            
            static string getTurn();
            static string getLayoutCard(int col, int row);
            static Player* getPlayer(int i);     
            static void layoutClear();
            static int layoutSize();
            static int layoutStackSize(int i);
            static void popFromStack(int col);
            static void setTurn(string turn);


 
            


};
#endif