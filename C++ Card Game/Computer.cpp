    /************************************************************
     * Name:  Adam Roberts                                      *
     * Project:  Project 1 / GO STOP C++                        *
     * Class:  CMPS 366 01                                      *
     * Date:  February 18, 2020                                 *
     ************************************************************/

#include "Player.h"
#include "Computer.h"
#include "GameBoard.h"
#include <iostream> 
#include <vector>
#include <algorithm> 

/* *********************************************************************
Function Name: Computer
Purpose: Constructor for Computer
Parameters: None
Return Value: Void
Local Variables: None
         
Algorithm:
            1) Assign suggested move to card played
            2) Remove card from hand
            3) Map Layout for last match, and stack occurences
            4) Determine move based on stack occurences
             4a) If no stacks, add to end of layout and call move2a.
             4b) If 1 or 2 stacks, add to last stack found and call move2b.
             4c) If 3 cards total, capture all 4 cards and call move2a. 

Assistance Received: none
********************************************************************* */
Computer::Computer() {
    userName = "Hal";

}
/* *********************************************************************
Function Name: move
Purpose: To execute the first of 2 moves a player makes
Parameters: None
Return Value: Void
Local Variables:
            card, current card being used
            count, count of matches in layout
            colLoc, location of last match found
            itS, string iterator for deleting from players hand
            toDel[], vector storing stacks to be delete            
Algorithm:
            1) Assign suggested move to card played
            2) Remove card from hand
            3) Map Layout for last match, and stack occurences
            4) Determine move based on stack occurences
             4a) If no stacks, add to end of layout and call move2a.
             4b) If 1 or 2 stacks, add to last stack found and call move2b.
             4c) If 3 cards total, capture all 4 cards and call move2a. 

Assistance Received: none
********************************************************************* */
void Computer::move(){
    string card;
    int count = 0;
    double colLoc;
    vector<string>::iterator itS; 
    vector<int> toDel;

    //Assign card based off sugestedMove
    card = suggestedMove();
    cout << "Hal has played a " << card << ". ";
    //Find card in hand and delete.
    itS = find(hand.begin(), hand.end(), card );
    hand.erase(itS);

    //Map layout for stacks matching card played. Running tally of count for switch
    //and map for multiple stacks.
    for (int col = 0; col < GameBoard::layoutSize(); col++)                                                      
    {
        if(GameBoard::getLayoutCard(col, 0).at(0) == card.at(0)){                              
            count += GameBoard::layoutStackSize(col);                                                                     
            colLoc = col;                                                                                                                                
        }
    }
    //Switch based on card quatity     
    switch(count){
    //Add card to end of layout        
    case 0:
        GameBoard::addLayoutEnd(card);
        cout << "That card was been added to end of layout.\n";
        matchingPair == -1; 
        move2a();
        break;
    //Add card to last layout match found
    case 1:
    case 2:
        GameBoard::addLayoutStack(colLoc, card);
        matchingPair = colLoc;                                
        cout << "Adding it to a matching card.\n";
        move2b();
        break;
    //Capture 4 cards
    default:                                         
        int cards;
        int cardsRem = 3;
        //col = 0;
        int size = GameBoard::layoutSize();
        //Pop from back of stack, leaving final card in vector
        for (double col = 0; col < size; ++col)
        {
            if(GameBoard::getLayoutCard(col, 0).at(0) == card.at(0)){
                while(GameBoard::getLayoutCard(col, 0).at(0) == card.at(0) && cardsRem !=0){
                    if( GameBoard::layoutStackSize(col) > 1 ){
                        GameBoard::popFromStack(col);
                        cardsRem--;
                    }
                    //store location for later deletion
                    if(GameBoard::layoutStackSize(col) == 1){
                        toDel.insert( toDel.begin() , col);
                        cardsRem--;
                        size--;
                        break;
                    }
                }
            }
        }
        //Delete stored locations        
        for(int i = 0; i< toDel.size(); i++){
            GameBoard::eraseStack(toDel[i]);
        }
        points++;
        matchingPair == -1; 
        cout << "Hal captured 4 cards and earns a point!\n";
        move2a();
        break;
    } 
}
/* *********************************************************************
Function Name: move2a
Purpose: To execute the first of 2 possible moves for 2nd move.
Parameters: None
Return Value: Void
Local Variables:
            card, current card being used
            count, count of matches in layout
            colLoc, location of last match found
            itC, char iterator for finding pairs
            toDel[], vector storing stacks to be delete
            temp, string for holding user input to end turn            
Algorithm:
            1) Remove card from playDeck
            2) Map Layout for last match, and stack occurences
            3) Determine move based on stack occurences
             3a) If no stacks, add to end of layout.
             3b) If 1 or 2 stacks, remove matches and check for pairs.
             3c) If 3 cards total, capture all 4 cards. 

Assistance Received: none
********************************************************************* */
void Computer::move2a(){
    string card = Deck::getDeckCard(0);
    int count = 0;
    double colLoc;
    vector<char>::iterator itC;
    vector<int> toDel;
    string temp;
    

    Deck::removeTopCard();
    cout << "Hal drew a " << card << " from the deck.\n";
    //Map layout for stacks matching card played. Running tally of count for switch
    //and map for multiple stacks.
    for (int col = 0; col < GameBoard::layoutSize(); col++)                                                               
    {
        if(GameBoard::getLayoutCard(col, 0).at(0) == card.at(0)){                          
            count += GameBoard::layoutStackSize(col);                                      
            colLoc = col;                                                                                                                                   
        }
    }
    //Switch based on card quantity   
    switch(count){
        //Add card to end of layout
        case 0:          
            GameBoard::addLayoutEnd(card);
            cout << "That card was been added to end of layout..\n";
            break;
        //Remove match made and check for matching pairs
        case 1: 
            itC = find(pairs.begin(), pairs.end() , card.at(0) );                
            if(itC != pairs.end() )                                                            
            {
            
                pairs.erase(itC);
                points++;
                cout << "He used it to capture a pair, and earned a point!\n";
            }
            else{                                                                        
                pairs.push_back(card.at(0));
                cout << "He used it to capture a pair.\n";
            }
            GameBoard::eraseStack(colLoc);           
            break;        
        case 2:                                   
            itC = find(pairs.begin(), pairs.end() , card.at(0) );                
            if(itC != pairs.end() )                                                            
            {
            
                pairs.erase(itC);
                points++;
                cout << "He used it to capture a pair, and earned a point!\n";
            }
            else{                                                                        
                pairs.push_back(card.at(0));
                cout << "He used it to capture a pair.\n";
            }
            GameBoard::eraseStack(colLoc);           
            break;
        //remove 4 matching cards
        default:                                         
            int cardsRem = 3;
            int size = GameBoard::layoutSize();
            //Pop from back of stack, leaving final card in vector
            for (double col = 0; col < size; ++col)
            {
                if(GameBoard::getLayoutCard(col, 0).at(0) == card.at(0)){
                    while(GameBoard::getLayoutCard(col, 0).at(0) == card.at(0) && cardsRem !=0){
                        if( GameBoard::layoutStackSize(col) > 1 ){
                            GameBoard::popFromStack(col);
                            cardsRem--;
                        }
                        //store location for later deletion
                        if(GameBoard::layoutStackSize(col) == 1){
                            toDel.insert( toDel.begin() , col);
                            cardsRem--;
                            size--;
                            break;
                        }
                    }
                }
            }
            //Delete stored locations
            for(int i = 0; i< toDel.size(); i++){
                GameBoard::eraseStack(toDel[i]);    
            }                             
            points++;
            cout << "Hal captured 4 cards and earns a point!\n";
            break;   
    }
    GameBoard::setTurn("Human");

    if(GameBoard::getGameType()!=3){
        cout << "Press Enter to continue...";
        getline(cin, temp);
        if(temp == "save"){ GameBoard::saveGame(); }
        if(temp == "quit"){ exit(0); }
    }

}
/* *********************************************************************
Function Name: move2b
Purpose: To execute one of 2 possible moves for 2nd move.
Parameters: None
Return Value: Void
Local Variables:
            card, current card being used
            count, count of matches in layout
            colLoc, location of last match found
            itS, string iterator for deleting from players hand
            toDel[], vector storing stacks to be delete
            temp, string for holding user input to end turn          
Algorithm:
            1) Remove card from hand
            2) Map Layout for last match, and stack occurences
            4) Determine move based on stack occurences
             4a) If no stacks, add to end of layout and remove previous match. Check for pairs.
             4b) If 1 or 2 stacks, add to last stack found and remove both matches. Check for pairs.
             4c) If 3 cards total, capture all 4 cards. 

Assistance Received: none
********************************************************************* */
void Computer::move2b(){


    string card = Deck::Deck::getDeckCard(0);
    int count = 0;
    int cards;
    int colLoc;
    vector<char>::iterator itC;
    string temp;
    vector<int> toDel;

    Deck::removeTopCard();

    cout << "Hal drew a " << card << " from the deck.\n";
    //Map layout for stacks matching card played. Running tally of count for switch
    //and map for multiple stacks.
    for(int col = 0; col < GameBoard::layoutSize(); col++){  
        if(GameBoard::getLayoutCard(col, 0).at(0) == card.at(0)){    
            count += GameBoard::layoutStackSize(col); 
            colLoc = col;                                                                                                                                   
        }
    }
    //Switch based on card quantity
    switch(count){
    //Add card to end of layout. Capture previous pairs
    case 0:           
        GameBoard::addLayoutEnd(card);
        if(matchingPair < 0 ){                                                             
            cout << "That card was been added to end of the layout..\n";
        }
        else{  
            cout << "He added it to the end of the layout ";                                                                          
            card = GameBoard::getLayoutCard(matchingPair, 0);
            itC = find(pairs.begin(), pairs.end() , card.at(0) );                  
            if(itC != pairs.end() ){                                              
                pairs.erase(itC);
                points++;
                cout << " and captured his previous pair, earning a point!\n";
            }
            else{                                                                      
                pairs.push_back(card.at(0));
                cout << "and captured his previous pair.\n";
            }
            GameBoard::eraseStack(matchingPair);
        }
        break;
    //Add card to (user defined match and collect both turn pairs)
    //or (leave 3 stacked).  
    case 1:
        cout << "Card added to a matching pair.\n";
        //Check for matching pairs and remove 
        itC = find(pairs.begin(), pairs.end() , card.at(0) );                           
        if(itC != pairs.end() )                                                         
        {
            pairs.erase(itC);
            points++;
            cout << "He used it to capture a matching pair, earning a point!\n";
        }
        else{                                                                                   
            pairs.push_back(card.at(0));
            cout << "He used it to capture a matching pair.\n";
        }
        GameBoard::eraseStack(colLoc);  
        if(colLoc<matchingPair){matchingPair--;}                                    
        card = GameBoard::getLayoutCard(matchingPair, 0);                                   
        //Check previous match for matching pairs and remove                          
        itC = find(pairs.begin(), pairs.end() , card.at(0) );                  
        if(itC != pairs.end() )                                            
        {
        
            pairs.erase(itC);
            points++;
            cout << "Hal also captured his previous matching pair, and earned a point!\n";
        }
        else{                                                       
            pairs.push_back(card.at(0));
            cout << "Hal also captured his previous matching pair.\n";
        }
        GameBoard::eraseStack(matchingPair);
 
                                                
        break;
    case 2:
        if(colLoc==matchingPair){
            cout << "Card added to previous match.\n";
            GameBoard::addLayoutStack(matchingPair, card);
        }
        else{
            cout << "Card added to a matching pair.\n";
            //Check for matching pairs and remove 
            itC = find(pairs.begin(), pairs.end() , card.at(0) );                           
            if(itC != pairs.end() )                                                         
            {
                pairs.erase(itC);
                points++;
                cout << "He used it to capture a matching pair, earning a point!\n";
            }
            else{                                                                                   
                pairs.push_back(card.at(0));
                cout << "He used it to capture a matching pair.\n";
            }
            GameBoard::eraseStack(colLoc);  
            if(colLoc<matchingPair){matchingPair--;}                                    
            card = GameBoard::getLayoutCard(matchingPair, 0);                                   
            //Check previous match for matching pairs and remove                          
            itC = find(pairs.begin(), pairs.end() , card.at(0) );                  
            if(itC != pairs.end() )                                            
            {
            
                pairs.erase(itC);
                points++;
                cout << "Hal also captured his previous matching pair, and earned a point!\n";
            }
            else{                                                       
                pairs.push_back(card.at(0));
                cout << "Hal also captured his previous matching pair.\n";
            }
            GameBoard::eraseStack(matchingPair);
 
        }                                         
        break;
    //remove 4 matching cards 
    default:                                         
        int cards;
        int cardsRem = 3;
        //Pop from back of stack, leaving final card in vector
        for (double col = 0; col < GameBoard::layoutSize(); ++col)
        {
            if(GameBoard::getLayoutCard(col, 0).at(0) == card.at(0)){
                    while(GameBoard::getLayoutCard(col, 0).at(0) == card.at(0) && cardsRem !=0){
                        if( GameBoard::layoutStackSize(col) > 1 ){
                            GameBoard::popFromStack(col);
                            cardsRem--;
                        }
                        if(GameBoard::layoutStackSize(col) == 1){
                            toDel.insert( toDel.begin() , col);
                            cardsRem--;
                            break;
                        }
                    }
            }
        }
        for(int i = 0; i< toDel.size(); i++){
            GameBoard::eraseStack(toDel[i]);  
            if(toDel[i]<matchingPair){ matchingPair--;}
            if(toDel[i]==matchingPair){ matchingPair = -1; }  
        }
         //Check to see if previous pair was in 4 cards removed
        if(matchingPair>-1) {
            string card = GameBoard::getLayoutCard(matchingPair, 0);                            
            itC = find(pairs.begin(), pairs.end() , card.at(0) );                           
            if(itC != pairs.end() )                                                             
            {
                pairs.erase(itC);
                points++;
                cout << "Hal captured his previous pair, and earned a point!\n";
            }
            else{                                                                       
                pairs.push_back(card.at(0));
                cout << "Hal captured his previous pair.\n";

            }
            GameBoard::eraseStack(matchingPair);    
        }                                                                
        points++;
        cout << "Hal captured 4 cards and earn a point!\n";
        break;
    }
    GameBoard::setTurn("Human");
    if(GameBoard::getGameType()!=3){
        cout << "Press Enter to continue...";
        getline(cin, temp);
        if(temp == "save"){ GameBoard::saveGame(); }
        if(temp == "quit"){ exit(0); }
    }
}
/* *********************************************************************
Function Name: printHand
Purpose: To print players hand.
Parameters: None
Return Value: Void
Local Variables:
         
Algorithm:
            1) Check for debug mode
            2a) If yes, print as is.
            2b) In no, print as 🃟

Assistance Received: none
********************************************************************* */
void Computer::printHand(){
    //If in debug mode show cards
    if(GameBoard::getDebug() == true){
        for(int i = 0; i< hand.size(); i++){
            cout << hand.at(i) << " ";
        }
    }
    //If not disguise them from user.
    else{
        for(int i = 0; i < hand.size() ; i++){
            cout << "🃟 ";
        } 
    }      
}
/* *********************************************************************
Function Name: suggested move
Purpose: To dictate move made by player.
Parameters: None
Return Value: Void
Local Variables:
            pairMatch, stores last match with a pair found.
            match, stores last pair found.
            count, running tally of matching cards found.
         
Algorithm:
            1) Check that layout contains cards
            2) Map Layout for matches of each card in hand
            3) If cards match, store in match
            4) If count equals 3, return current card
            5) If match is in pairs, store in pairMatch
            6) If pairMatch found, return that.
            7) If match found, return that.
            8) If nothing return first card in deck.

Assistance Received: none
********************************************************************* */
string Computer::suggestedMove(){
    //string to hold last fund pair match
    string pairMatch = "x";
    //string to hold last found match
    string match = "x"; 
    //int to hold total matches found in layout
    int count = 0;
    
    //If no cars in layout, return first card
    if(GameBoard::layoutSize() == 0){ return hand[0]; }
    
    //Check each card in hand
    for(int j = 0;j<hand.size();j++){   
        count = 0;
        //Against each top card in layout
        for(int k = 0; k < GameBoard::layoutSize();k++){ 
            //If cards match  
            if(hand[j].at(0) == GameBoard::getLayoutCard(k, 0).at(0) ){ 
                //increase match count
                count += GameBoard::layoutStackSize(k);
                //set match found current card
                match = hand[j];
                //If count is 3          
                if(count == 3){
                    //Immediately return current card
                    return hand[j];
                }
                //Compare against caught pairs
                for(int i = 0; i< pairs.size(); i++){
                    //If pair matches current card
                    if(pairs[i]==GameBoard::getLayoutCard(k, 0).at(0)){
                        //set pairMatch to current card
                        pairMatch == hand[j];
                    }
                }

            }
        }
    }   
    //Return last pair match found if any.
    if(pairMatch!="x"){
        return pairMatch;
    }
    //If none return last match found
    else if(match!="x"){
        return match;
    }
    //If none, return first hand found
    else{
        return hand[0];
    }    
}