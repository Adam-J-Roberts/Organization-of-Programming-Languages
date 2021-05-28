    /************************************************************
     * Name:  Adam Roberts                                      *
     * Project:  Project 1 / GO STOP C++                        *
     * Class:  CMPS 366 01                                      *
     * Date:  February 18, 2020                                 *
     ************************************************************/

#include "Dealer.h"
#include "Deck.h"
#include "GameBoard.h"


using namespace std;
/* *********************************************************************
Function Name: Dealer
Purpose: Constructor for Dealer Class.  Deal cards and shuffle them.
Parameters: None
Return Value: Object of the Dealer class
Local Variables: None
               
Algorithm:
            1) While playersTurn is undefined
             1a) Deal cards 
            2) Shuffle cards


Assistance Received: none
********************************************************************* */
Dealer::Dealer()
{
    do{
        deal();
    }while(GameBoard::getTurn() == "undefined");
    Deck::shuffle();

}
/* *********************************************************************
Function Name: deal
Purpose: To shuffle and distribute the cards accordingly.
Parameters: None
Return Value: Void
Local Variables:
                doubleDecker, a Deck pointer holding 2 decks of cards
Algorithm:
            1) while Players turn is undefined, 
              2a) Shuffle deck.
              2b) distribute 5 cards to each player
              2c) distribute 4 cards to the layout
              2d) Repeat 2b and 2c
            3)determin players turn

Assistance Received: none
********************************************************************* */
void Dealer::deal()
{   
    Deck* doubleDecker;
    doubleDecker = new Deck(2);
    GameBoard::getPlayer(0)->handClear();
    GameBoard::getPlayer(1)->handClear();
    GameBoard::getPlayer(0)->pairsClear();
    GameBoard::getPlayer(1)->pairsClear();
    GameBoard::layoutClear();

    do{
        Deck::shuffle();
        for(int k = 0; k<2; k++){
            for(int i = 0; i <5; i++){ 
                GameBoard::getPlayer(0)->drawCard();
                GameBoard::getPlayer(1)->drawCard();
            }
            
            for(int j = 0; j <4; j++){ 
                GameBoard::drawLayout();    
            }
        }
        determinTurn();
    }while(GameBoard::getTurn() == "undefined");

}
/* *********************************************************************
Function Name: determinTurnd
Purpose: To determine who goes first.
Parameters: None
Return Value: Void
Local Variables:
                value[], a char array holding the possible matches in 
                    ascending order
                User[], in array holding total of matched pairs.
Algorithm:
            1) Set playerTurn to  who has highest point total if any.
            2) Set playerTurn to who has the highest card.
            3) Leave as undefined if both are equal.

Assistance Received: none
********************************************************************* */
void Dealer::determinTurn()
{
    GameBoard::setTurn("Unidentified");
    char value[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'X', 'J', 'Q', 'K'};
    if(GameBoard::getPlayer(0)->getPoints() > GameBoard::getPlayer(1)->getPoints() ){ 
        GameBoard::setTurn("Human"); 
        return; 
    }
    if(GameBoard::getPlayer(0)->getPoints() < GameBoard::getPlayer(1)->getPoints() ){ 
        GameBoard::setTurn("Computer");  
        return;
    } 
    for(int i=12; i!=0; i--){                                            
        int User[] = {0, 0};                           
        for(int j=0; j<2;j++){                                               
            for(int k=0; k<GameBoard::getPlayer(j)->handSize(); k++){     
                if(GameBoard::getPlayer(j)->getCard(k).at(0) == value[i] ){   
                    User[j]++;                                                
                }                                                    
            }
        }

        if(User[0]>User[1]){ 
            GameBoard::setTurn("Human");}
            return;
        if(User[0]<User[1]){ 
            GameBoard::setTurn("Computer"); 
            return;
        } 
    }
}
