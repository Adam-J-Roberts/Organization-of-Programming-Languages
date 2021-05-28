    /************************************************************
     * Name:  Adam Roberts                                      *
     * Project:  Project 1 / GO STOP C++                        *
     * Class:  CMPS 366 01                                      *
     * Date:  February 18, 2020                                 *
     ************************************************************/

#include "Human.h"
#include "GameBoard.h"
#include <iostream> 
#include <algorithm>
#include <map>


/* *********************************************************************
Function Name: Human
Purpose: Constructor for Human class
Parameters: None
Return Value: Object
Local Variables:
        
Algorithm:
            1) Prompt user for player name
            2) Set userName to user input.

Assistance Received: none
********************************************************************* */
Human::Human(){
    cout << "Please enter a name: ";
    cin >> userName;
    cin.ignore();
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
            choice, string to check for save or quit
            itS, string iterator for deleting from players hand
            map<>, map to hold card name and location
            toDel[], vector storing stacks to be delete            
Algorithm:
            1) Prompt user for card to be played
            2) Remove card from hand
            3) Map Layout for last match, and stack occurences
            4) Determine move based on stack occurences
             4a) If no stacks, add to end of layout and call move2a.
             4b) If 1 or 2 stacks, add to last stack found and call move2b.
             4c) If 3 cards total, capture all 4 cards and call move2a. 

Assistance Received: none
********************************************************************* */
void Human::move(){
    string card;
    int count = 0;
    int colLoc;
    string choice;
    vector<string>::iterator itS;
    map<string, int> map;
    vector<int> toDel;
    
    // Check for debug mode. Ignores asking for user input for testing purposes.
    if(GameBoard::getGameType()!=3){
        cout << "What card would you like to play? ";
        do{                 
            cin >> card;
            cin.ignore();
            if(card == "help"){ GameBoard::getPlayer(0)->suggestedMove(); }
            if(card == "save"){ GameBoard::saveGame(); }
            if(card == "quit"){ exit(0); }
        }while( (itS = find(hand.begin(), hand.end(), card )) == hand.end() );
        hand.erase(itS);
    }
    else{
        card = GameBoard::getPlayer(0)->suggestedMove();
        itS = find(hand.begin(), hand.end(), card );
        hand.erase(itS);
    }
    //Map layout for stacks matching card played. Running tally of count for switch
    //and map for multiple stacks.
    for (int col = 0; col < GameBoard::layoutSize(); col++)                                                  
    {
        if(GameBoard::getLayoutCard(col, 0).at(0) == card.at(0)){                                          
            count += GameBoard::layoutStackSize(col);                                                              
            map[GameBoard::getLayoutCard(col, 0)] = col;                         
            colLoc = col; 
            //topCards++;                                                                                                                                  
        }
    }
    //Switch based on card quatity 
    switch(count){
    //Add card to end of layout
    case 0:
            GameBoard::addLayoutEnd(card);
            cout << "Card added to end of layout.\n";
            matchingPair == -1; 
            move2a();
        break;
    //Add card to layout match
    case 1:
        GameBoard::addLayoutStack(colLoc, card);
        matchingPair = colLoc;
        cout << "Card added to a matching card.\n";
        move2b();
        break;
    //Prompt user for which card in layout to match
    case 2:
        //If statement only for hidden debugging mode
        if(GameBoard::getGameType()!=3){
            cout << "Which card would you like to match your "<< card << " with? :";  
            do{                 
                cin >> choice;
                cin.ignore();
                if(choice == "quit"){ exit(0); }
                if(choice == "save"){ cout << "Please wait till beginning or end of turn.\n";}
                if(choice == "help"){ cout << "All choices are equal. Go ahead and drop it on " << GameBoard::getLayoutCard(colLoc, 0) << ".\n"; }
            }while( map.find(choice) == map.end() );
            colLoc = map[choice];
        }

        GameBoard::addLayoutStack(colLoc, card);
        matchingPair = colLoc;                                                       
        cout << "Adding it to a matching card.\n";
        move2b();
        break;
    //Capture 4 cards
    default:                                         
        int cards;
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
        matchingPair == -1; 
        cout << "You have captured 4 cards and earn a point!\n";
        move2a();
        break;
    }
    GameBoard::setTurn("Computer"); 
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
            choice, string to check for save or quit
            itC, char iterator for checking for pairs
            map<>, map to hold card name and location
            toDel[], vector storing stacks to be delete          
Algorithm:
            1) Remove card from playDeck
            2) Map Layout for last match, and stack occurences
            3) Determine move based on stack occurences
             3a) If no stacks, add to end of layout.
             3b) If 1 or 2 matches, remove current match and check for pairs.
             3c) If 3 cards total, capture all 4 cards. 

Assistance Received: none
********************************************************************* */
void Human::move2a(){
    string card = Deck::getDeckCard(0);
    int count = 0;
    int colLoc;
    string choice;    
    vector<char>::iterator itC;     
    map<string, int> map;
    vector<int> toDel;

    Deck::removeTopCard();
    cout << "You drew a " << card << " from the deck.\n";
    //Map layout for stacks matching card played. Running tally of count for switch
    //and map for multiple stacks.
    for (int col = 0; col < GameBoard::layoutSize(); col++)                                                     
    {
        if(GameBoard::getLayoutCard(col, 0).at(0) == card.at(0)){                              
            count += GameBoard::layoutStackSize(col);                                                                      
            map[GameBoard::getLayoutCard(col, 0)] = col;                                                  
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
                cout << "You have captured a pair, and earned a point!\n";
            }
            else{                                                                         
                pairs.push_back(card.at(0));
                cout << "You have captured a pair.\n";
            }
            GameBoard::eraseStack(colLoc);      
            break;
        //Remove match selected by user and check for matching pairs
        case 2:
            //For debugging purposes only
            if(GameBoard::getGameType()!=3){
                cout << "Which card would you like to match your "<< card << " with? :";    
                do{                 
                    cin >> choice;
                    cin.ignore();
                    if(choice == "quit"){ exit(0); }
                    if(choice == "save"){ cout << "Please wait till beginning or end of turn.\n";}
                    if(choice == "help"){ cout << "All choices are equal. Go ahead and drop it on " << GameBoard::getLayoutCard(colLoc, 0) << ".\n"; }
                }while( map.find(choice) == map.end() );
                colLoc = map[choice];
            }
            itC = find(pairs.begin(), pairs.end() , card.at(0) );  
            if(itC != pairs.end() )                                        
            {
                pairs.erase(itC);
                points++;
                cout << "You have captured a pair, and earned a point!\n";
            }
            else{                                                          
                pairs.push_back(card.at(0));
                cout << "You have captured a pair.\n";
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
            cout << "You have captured 4 cards and earn a point!\n";
            break;   
    }

    string temp;
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
            choice, string to check for save or quit
            itC, char iterator for checking for pairs
            map<>, map to hold card name and location
            toDel[], vector storing stacks to be delete         
Algorithm:
            1) Remove card from hand
            2) Map Layout for last match, and stack occurences
            4) Determine move based on stack occurences
             4a) If no stacks, add to end of layout and remove previous match. Check for pairs.
             4b) If 1 or 2 stacks, add to user defined stack  and remove both matches. Check for pairs.
             4c) If 3 cards total, capture all 4 cards and previous matching pair. Check for pairs. 

Assistance Received: none
********************************************************************* */
void Human::move2b(){

    string card = Deck::Deck::getDeckCard(0);
    int count = 0;
    int colLoc;
    string choice;
    vector<char>::iterator itC;     
    map<string, int> map;    
    vector<int> toDel;

    cout << "You drew a " << card << " from the deck.\n";
    Deck::removeTopCard();
    //Map layout for stacks matching card played. Running tally of count for switch
    //and map for multiple stacks.
    for(int col = 0; col < GameBoard::layoutSize(); col++){                                                             
        if(GameBoard::getLayoutCard(col, 0).at(0) == card.at(0)){                                   
            count += GameBoard::layoutStackSize(col);                                                                            
            map[GameBoard::getLayoutCard(col, 0)] = col;                                      
            colLoc = col;                                                                                                                                 
        }
    }
    //Switch based on card quantity
    switch(count){
    //Add card to end of layout. Capture previous pairs
    case 0:  
        GameBoard::addLayoutEnd(card);
        cout << "That card was been added to end of the layout ";                                                                                                             
        card = GameBoard::getLayoutCard(matchingPair, 0);
        itC = find(pairs.begin(), pairs.end() , card.at(0) );               
        if(itC != pairs.end() ){                                         
            pairs.erase(itC);
            points++;
            cout << " and captured the previous pair, earning a point!\n";
        }
        else{                                                                            
            pairs.push_back(card.at(0));
            cout << "and capturing previous pair.\n";
        }
        GameBoard::eraseStack(matchingPair);
        break;
    //Add card to layout match and collect both turn pairs
    case 1:
        itC = find(pairs.begin(), pairs.end() , card.at(0) );                       
        if(itC != pairs.end() )                                                   
        {
            pairs.erase(itC);
            points++;
            cout << "You have captured a matching pair, and earned a point!\n";
        }
        else{                                                                     
            pairs.push_back(card.at(0));
            cout << "You have captured a pair.\n";
        }
        GameBoard::eraseStack(colLoc);  
        if(colLoc<matchingPair){matchingPair--;}                                   
        card = GameBoard::getLayoutCard(matchingPair, 0);
        if(matchingPair >=0 ){                                                              
            string card = GameBoard::getLayoutCard(matchingPair, 0);                                         
            itC = find(pairs.begin(), pairs.end() , card.at(0) );                   
            if(itC != pairs.end() )                                                       
            {
            
                pairs.erase(itC);
                points++;
                cout << "You have captured your previous matching pair, and earned a point!\n";
            }
            else{                                                          
                pairs.push_back(card.at(0));
                cout << "You also captured your previous matching pair.\n";
            }
            GameBoard::eraseStack(matchingPair);
        }  
        break;
    //Add card to (user defined match and collect both turn pairs)
    //or (leave 3 stacked).
    case 2:
        if(colLoc==matchingPair){
            cout << "Card added to previous match.\n";
            GameBoard::addLayoutStack(matchingPair, card);
        }    
        else{
            //For debugging purposes only
            if(GameBoard::getGameType()!=3){
                cout << "Which card would you like to match your "<< card << " with? :";     
                //Do while selection is a correct option
                do{                 
                    cin >> choice;
                    cin.ignore();
                    //Persistent menu options.
                    if(choice == "quit"){ exit(0); }
                    if(choice == "save"){ cout << "Please wait till beginning or end of turn.\n";}
                    if(choice == "help"){ cout << "All choices are equal. Go ahead and drop it on " << GameBoard::getLayoutCard(colLoc, 0) << ".\n"; }
                }while( map.find(choice) == map.end() );
                colLoc = map[choice];
            }
            //Check for matching pairs and remove          
            itC = find(pairs.begin(), pairs.end() , card.at(0) );                       
            if(itC != pairs.end() )                                              
            {
                pairs.erase(itC);
                points++;
                cout << "You have captured a pair, and earned a point!\n";
            }
            else{                                                               
                pairs.push_back(card.at(0));
                cout << "You have captured a matching pair.\n";
            }
            GameBoard::eraseStack(colLoc); 
            //Check previous match for matching pairs and remove                                                                                
            if(colLoc<matchingPair){matchingPair--;}   
            string card = GameBoard::getLayoutCard(matchingPair, 0);                            
            itC = find(pairs.begin(), pairs.end() , card.at(0) );                           
            if(itC != pairs.end() )                                                             
            {
                pairs.erase(itC);
                points++;
                cout << "You have also captured your previous pair, and earned a point!\n";
            }
            else{                                                                       
                pairs.push_back(card.at(0));
                cout << "You have also captured your previous pairr.\n";

            }
            GameBoard::eraseStack(matchingPair);
        }       
        break;
    //remove 4 matching cards   
    default:                                         
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
                cout << "You have  captured your previous pair, and earned a point!\n";
            }
            else{                                                                       
                pairs.push_back(card.at(0));
                cout << "You have captured your previous pair.\n";

            }
            GameBoard::eraseStack(matchingPair);    
        } 
        points++;
        cout << "You have captured 4 cards and earn a point!\n";
        break;
    }
    string temp;
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
            1) Cycle through each card in hand
            2) Display said card


Assistance Received: none
********************************************************************* */
void Human::printHand(){
    for(int i = 0; i< hand.size(); i++){
        cout << hand.at(i) << " ";
    }
}
/* *********************************************************************
Function Name: printPairs
Purpose: To print players pairs.
Parameters: None
Return Value: Void
Local Variables:
         
Algorithm:
            1) Sort pairs
            2) Cycle through each card in hand
             2a) Display said card
2
Assistance Received: none
********************************************************************* */
void Human::printPairs(){
    
    sort(pairs.begin(), pairs.end());

    for(int i = 0; i< pairs.size(); i++){
        cout << pairs.at(i) << " ";
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
             1a) Suggest any card
            2) Map Layout for matches of each card in hand
            3) If cards match, store in match
            4) If count equals 3, return current card and suggest.
            5) If match is in pairs, store in pairMatch
            6) If pairMatch found, return that card and suggest it.
            7) If match found, return that and suggest.
            8) If nothing return first card in deck and suggest it.

Assistance Received: none
********************************************************************* */
string Human::suggestedMove(){
    //string to hold last fund pair match
    string pairMatch = "x";
    //string to hold last found match
    string match = "x"; 
    //int to hold total matches found in layout
    int count = 0;
    
    //If no cars in layout, return first card
    if(GameBoard::layoutSize() == 0){ 
        cout << "Feel free to play any card.";
        return hand[0]; 
    }
    //Check each card in hand
    for(int j = 0;j<hand.size();j++){ 
        count = 0;
        //Against each top card in layout       
        for(int k = 0; k< GameBoard::layoutSize();k++){
            //If cards match
            if(hand[j].at(0) == GameBoard::getLayoutCard(k, 0).at(0) ){
                //increase match count
                count+=GameBoard::layoutStackSize(k);
                //set match found current card
                match = hand[j]; 
                //If count is 3                                 
                if(count == 3){
                    cout << "Playing your " << hand[j] << " would match it with 3 other cards.\n";
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
        cout << "Playing your " << pairMatch << " would give you two matches and a point.\n";
        return pairMatch;
    }
    //If none return last match found
    if(match!="x"){
        cout << "Playing your " << match << " would stack with another card in the deck.\n";
        return match;
    }
    //If none, return first hand found
    cout << "You have no moves that would be better than others.\n";
    return hand[0];
}





    