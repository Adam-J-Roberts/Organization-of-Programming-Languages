    /************************************************************
     * Name:  Adam Roberts                                      *
     * Project:  Project 1 / GO STOP C++                        *
     * Class:  CMPS 366 01                                      *
     * Date:  February 18, 2020                                 *
     ************************************************************/

#include "Player.h"
#include "Deck.h"


/* *********************************************************************
Function Name: Player
Purpose: Constructor of Player class
Parameters: None
Return Value: Object of Player class
Local Variables: None
         
Algorithm:

Assistance Received: none
********************************************************************* */
Player::Player(){

}
/* *********************************************************************
Function Name: addCard
Purpose: Add card to hand
Parameters: string card
Return Value: Void
Local Variables: None
         
Algorithm:
            1) call push_back on hand with card

Assistance Received: none
********************************************************************* */
void Player::addCard(string card){
    hand.push_back(card);
}
/* *********************************************************************
Function Name: addPair
Purpose: add card to captured pairs
Parameters: char card
Return Value: Void
Local Variables: None
         
Algorithm:
            1) call push_back on pair with card

Assistance Received: none
********************************************************************* */
void Player::addPair(char card){
    pairs.push_back(card);
}
/* *********************************************************************
Function Name: addPoints
Purpose: Increment points
Parameters: None
Return Value: Void
Local Variables: None
         
Algorithm:
            1) Increment points

Assistance Received: none
********************************************************************* */
void Player::addPoints(){
    points++;
}
/* *********************************************************************
Function Name: drawCard
Purpose: Draw top deck card and add it to players hand.
Parameters: None
Return Value: Void
Local Variables: None
         
Algorithm:
            1) Call push_back on hand with first card in Deck.
            2) Call removeTopCard

Assistance Received: none
********************************************************************* */
void Player::drawCard(){
    hand.push_back( Deck::getDeckCard(0) );
    Deck::removeTopCard();
}
/* *********************************************************************
Function Name: getCard
Purpose: Getter for card in hand at loc
Parameters: int loc
Return Value: Void
Local Variables: None
         
Algorithm:
            1) Return hand.at(loc)

Assistance Received: none
********************************************************************* */
string Player::getCard(int loc){
    return hand.at(loc);
}
/* *********************************************************************
Function Name: getName
Purpose: Getter for userName
Parameters: None
Return Value: Void
Local Variables: None
         
Algorithm:
            1) Return userName

Assistance Received: none
********************************************************************* */
string Player::getName(){
    return userName;
}
/* *********************************************************************
Function Name: getPair
Purpose: Getter for pairs at loc
Parameters: int loc
Return Value: Void
Local Variables: None
         
Algorithm:
            1) Return pairs[loc]

Assistance Received: none
********************************************************************* */
char Player::getPair(int loc){
    return pairs[loc];
}
/* *********************************************************************
Function Name: getPoints
Purpose: Getter for Points
Parameters: None
Return Value: Void
Local Variables: None
         
Algorithm:
            1) Return points

Assistance Received: none
********************************************************************* */
int Player::getPoints(){
    return points;
}
/* *********************************************************************
Function Name: handClear
Purpose: Clears the hand vector
Parameters: None
Return Value: Void
Local Variables: None
         
Algorithm:
            1) call clear() on hand

Assistance Received: none
********************************************************************* */
void Player::handClear(){
    hand.clear();
}
/* *********************************************************************
Function Name: handSize
Purpose: Getter for hand size.
Parameters: None
Return Value: Void
Local Variables: None
         
Algorithm:
            1) Return hand.size()
            
Assistance Received: none
********************************************************************* */
int Player::handSize(){
    return hand.size();
}
/* *********************************************************************
Function Name: pairsClear
Purpose: Clears the pairs vector
Parameters: None
Return Value: Void
Local Variables: None
         
Algorithm:
            1) Call clear() on pairs
            
Assistance Received: none
********************************************************************* */
void Player::pairsClear(){
    pairs.clear();
}
/* *********************************************************************
Function Name: setPoints
Purpose: Setter for points.
Parameters: int point
Return Value: Void
Local Variables: None
         
Algorithm:
            1) Assign point to points

Assistance Received: none
********************************************************************* */
void Player::setPoints(int point){
    points = point;
}
/* *********************************************************************
Function Name: pairSize
Purpose: Getter for pairs size
Parameters: None
Return Value: Void
Local Variables: None
         
Algorithm:
            1) Return pairs.size()

Assistance Received: none
********************************************************************* */
int Player::pairSize(){
    return pairs.size();
}
