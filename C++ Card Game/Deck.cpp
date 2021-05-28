    /************************************************************
     * Name:  Adam Roberts                                      *
     * Project:  Project 1 / GO STOP C++                        *
     * Class:  CMPS 366 01                                      *
     * Date:  February 18, 2020                                 *
     ************************************************************/

#include "Deck.h"
#include <iostream> 
#include <algorithm>

vector<string> Deck::playDeck;

/* *********************************************************************
Function Name: Deck
Purpose: Constructor for Deck
Parameters: int count
Return Value: Deck object
Local Variables:
                value[], vector holding possible values of card
                suite[], vector holding possible suites of card
         
Algorithm:
            1) Clear playDeck vector
            2) Loop through count times
            3) Loop throough possible values
            4) Loop through possible suites
            5) Create string of each possible card
            6) Push string onto Deck

Assistance Received: none
********************************************************************* */

Deck::Deck(int count) {   
    
    playDeck.clear();
    string value[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "X", "J", "Q", "K"};
    string suite[] = { "S", "H", "C", "D", }; 
    for(int z = 0; z < count; z++){
        for(int i=0; i<=3; i++){
            for(int j=0; j<=12; j++){
                string temp = value[j] + suite[i];
                playDeck.push_back(temp);
            }
        }
    } 
}
/* *********************************************************************
Function Name: addCard
Purpose: To push card to from of playDeck. Only used in loading games
Parameters: string card, card to be added to playDeck
Return Value: Void
Local Variables:
         
Algorithm:
            1) Call push_back() call.

Assistance Received: none
********************************************************************* */
void Deck::addCard(string card){
    playDeck.push_back(card);
}
/* *********************************************************************
Function Name: deckSize
Purpose: Getter for playDeck size
Parameters: None
Return Value: int, size of deck
Local Variables:
         
Algorithm:
            1) return playDeck.size()

Assistance Received: none
********************************************************************* */
int Deck::deckSize(){
    return playDeck.size();
}
/* *********************************************************************
Function Name: getDeckCard
Purpose: Getter for returning card in specific location
Parameters: int loc, the location of card wanted.
Return Value: String
Local Variables:
         
Algorithm:
            1) Return playDeck at location of loc 

Assistance Received: none
********************************************************************* */
string Deck::getDeckCard(int loc){
    return playDeck[loc];
}
/* *********************************************************************
Function Name: printDeck
Purpose: Helper function to print playDeck. Only used in debug Mode
Parameters: None
Return Value: Void
Local Variables:
         
Algorithm:
            1) Basic for loop printing each card

Assistance Received: none
********************************************************************* */
void Deck::printDeck(){
    for(int i = 0; i< playDeck.size(); i++){
        cout << playDeck.at(i) << " ";
    }
}
/* *********************************************************************
Function Name: removeTopCard
Purpose: Getter for top card of playDeck
Parameters: None
Return Value: Void
Local Variables:
         
Algorithm:
            1) remove top card of playDeck

Assistance Received: none
********************************************************************* */
void Deck::removeTopCard(){
    playDeck.erase(playDeck.begin(), playDeck.begin() + 1);
}
/* *********************************************************************
Function Name: shuffle
Purpose: To randomly shuffle the playDeck
Parameters: None
Return Value: Void
Local Variables:
         
Algorithm:
            1) Call random_shuffle function. 

Assistance Received: none
********************************************************************* */
void Deck::shuffle(){
    random_shuffle( playDeck.begin(), playDeck.end());
}









