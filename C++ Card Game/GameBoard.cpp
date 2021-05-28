    /************************************************************
     * Name:  Adam Roberts                                      *
     * Project:  Project 1 / GO STOP C++                        *
     * Class:  CMPS 366 01                                      *
     * Date:  February 18, 2020                                 *
     ************************************************************/


#include <iomanip>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "GameBoard.h"
#include "Human.h"
#include "Computer.h"
#include "Deck.h"


GameBoard::gameMode GameBoard::gameType;
vector<Player*> GameBoard::players;
vector< vector<string> > GameBoard::layout;
bool GameBoard::debugMode = false;
string GameBoard::playersTurn = "undefined";
double GameBoard::round = 0;

/* *********************************************************************
Function Name: GameBoard
Purpose: Constructor for GameBoard class
Parameters: None
Return Value: Object of GameBoard
Local Variables:
         
Algorithm:
            1) Determin Debug mode
            2) Determin Game mode
            3) Create players
            4) Start game type based on gameMode
            5) trigger endGame

Assistance Received: none
********************************************************************* */

GameBoard::GameBoard(){
    determinDebug();
    determinGameMode();
    
    system("clear");
    players.push_back( new Human() );   
    players.push_back( new Computer() );
    system("clear");

    switch(gameType){
        case(0):
            loadGame();
            break;
        case(1):
            playGame();
            break;
        case(2):
            playRounds();
            break;
        case(3):
            startDebug();

    }

    endGame();
}
/* *********************************************************************
Function Name: addLayoutEnd
Purpose: Adds selected card to end of layout
Parameters: string card
Return Value: Void
Local Variables:
                newRow[], temporary vector to hold new stacks on the layout.
         
Algorithm:
            1) Create temp vector holding the new card.
            2) Push it to the end of layout.

Assistance Received: none
********************************************************************* */
void GameBoard::addLayoutEnd(string card){
    vector<string> newRow{ card };
    layout.push_back(newRow);
}
/* *********************************************************************
Function Name: addLayoutStack(
Purpose: Adds selected card to stacked card
Parameters: int col, string card
Return Value: Void
Local Variables: 

Algorithm:
            1)call push_back with the selected card to the vector at the 
                col location.

Assistance Received: none
********************************************************************* */
void GameBoard::addLayoutStack(int col, string card){
    layout[col].push_back(card);
}
/* *********************************************************************
Function Name: determinDebug
Purpose:
Parameters: None
Return Value: Void
Local Variables:
                temp, string to hold user input.
Algorithm:
            1)clear screen 
            2)Prompt user for selection
            3)Validate input
            4)If yes, set debugMode to true, else to false.


Assistance Received: none
********************************************************************* */
void GameBoard::determinDebug(){
    
    string temp;
    system("clear");
    cout << "\nWould you like to play in debug mode?: Yes or No: ";
    cin >> temp;
    transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
    while(temp != "yes" && temp != "y" && temp != "no" && temp != "n") {
			cin.clear();
			cin.ignore();
			cout << "\nNot a valid choice. Input yes or no: ";
			cin >> temp;
            cin.ignore();
	}
    if(temp == "yes" || temp == "y"){ debugMode = true; }
    else if(temp == "no" || temp == "n"){ debugMode = false; }
}
/* *********************************************************************
Function Name: determinGameMode
Purpose: Set gameType based on user input.
Parameters: None
Return Value: Void
Local Variables:
                choice, temporary string holding user input
         
Algorithm:
            1)clear screen 
            2)Prompt user for selection
            3)Validate input
            4)set gameType based on selection

Assistance Received: none
********************************************************************* */
void GameBoard::determinGameMode(){
    
    system("clear");
    string choice;
    cout << "\nWould you like to play (r)ound,a (s)ingle game or (l)oad a previous game?: ";
    cin >> choice;
    while(choice != "r"&& choice != "s" && choice != "l" && choice != "d") {
        cin.clear();
        cin.ignore();
        cout << "\nNot a valid choice. Input r for rounds, s for a single game, or l to load game: ";
        cin >> choice;
        cin.ignore();
    }
    if(choice == "r"){gameType = roundRobin;}
    if(choice == "s"){gameType = game;}
    if(choice == "l"){gameType = load;}
    if(choice == "d"){gameType = debug;}
}
/* *********************************************************************
Function Name: displayBoard
Purpose: Displays the game board
Parameters: None
Return Value: Void
Local Variables:
         
Algorithm:
            1)  Display help menu
            2)  Display deck based on debugMode
            3)  Display computers hand based on debugMode
            4)  Display layout
            5)  Display users hand and pairs


Assistance Received: none
********************************************************************* */
void GameBoard::displayBoard(){
    
    if(GameBoard::gameType!=3 && debugMode == false){
        system("clear");
    }
    
    if(debugMode == true){
        cout << "Play Deck:\n";
        Deck::printDeck();
        cout << endl;
    }
    cout << "Enter help, save, or quit at anytime.\n";
    cout << "-------------------------------------------------------------------\n";
    cout << players[1]->getName();
    if(debugMode == false){ 
        cout << setw(55) << "Deck" << endl;
    }
    if(debugMode == true){ 
        cout << endl;
    }
    players[1]->printHand();
    
    if(debugMode == false){
        int k = 60 - (players[1]->handSize()*2); 
        cout << setw(k) << "🃟 x" << Deck::deckSize() << endl; 
    }
    


    cout << "\n\n\n\n";
    cout << "Layout\n";
    printlayout();
    cout << "\n\n";
    cout << players[0]->getName() << endl;
    players[0]->printHand();
    cout << endl << "Captured Pairs: ";
    players[0]->printPairs();
    cout << "\n-------------------------------------------------------------------\n";


}
/* *********************************************************************
Function Name: drawLayout
Purpose: draws top card form deck, dealing it to layout
Parameters: None
Return Value: Void
Local Variables:
                row[], vector holding top card
Algorithm:
            1) Create vector holding top deck card
            2) Push vector to end of layout
            3) Delete top card from deck

Assistance Received: none
********************************************************************* */
void GameBoard::drawLayout(){
   
    vector<string> row{ Deck::getDeckCard(0) };
    layout.push_back(row);
    Deck::removeTopCard();
}
/* *********************************************************************
Function Name: endGame
Purpose: Display end screen based on points.
Parameters: None
Return Value: Void
Local Variables:
                sizeComp, string to set spacing offset.
Algorithm:
            1)Compare scores
            2)display approriate end screen

Assistance Received: none
********************************************************************* */
void GameBoard::endGame(){
    system("clear");
    string sizeComp = to_string(getPlayer(0)->getPoints());
    if(getPlayer(1)->getPoints() > getPlayer(0)->getPoints()){
        cout <<    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
                << "XXXX  XXXXX  XX        XX  XXXX  XXXXXXX  XXXXXX        XX        XX       X   X\n"
                << "XXXXX  XXX  XXX  XXXX  XX  XXXX  XXXXXXX  XXXXXX  XXXX  XX  XXXXXXXX  XXXXXX   X\n"
                << "XXXXXX  X  XXXX  XXXX  XX  XXXX  XXXXXXX  XXXXXX  XXXX  XX  XXXXXXXX  XXXXXX  XX\n"
                << "XXXXXXX   XXXXX  XXXX  XX  XXXX  XXXXXXX  XXXXXX  XXXX  XX        XX      XX  XX\n"
                << "XXXXXXX   XXXXX  XXXX  XX  XXXX  XXXXXXX  XXXXXX  XXXX  XXXXXXXX  XX  XXXXXX XXX\n"
                << "XXXXXXX   XXXXX  XXXX  XX  XXXX  XXXXXXX  XXXXXX  XXXX  XXXXXXXX  XX  XXXXXXXXXX\n"
                << "XXXXXXX   XXXXX        XX        XXXXXXX      XX        XX        XX       X  XX\n"
                << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
                << "\n" << setw(51) << "You     vs      Hal\n"
                << setw(34) << getPlayer(0)->getPoints()<< setw(18-sizeComp.size()) << getPlayer(1)->getPoints() << endl << endl;
    }
    else if(getPlayer(1)->getPoints() < getPlayer(0)->getPoints()){
        cout <<    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
                << "XXXX  XXXXX  XX        XX  XXXX  XXXX  XXXXXXXXXXX  XX      XX    XXXX  XXX   XX\n"
                << "XXXXX  XXX  XXX  XXXX  XX  XXXX  XXXX  XXXXXXXXXXX  XXXX  XXXX  X  XXX  XXX   XX\n"
                << "XXXXXX  X  XXXX  XXXX  XX  XXXX  XXXX  XXXXXXXXXXX  XXXX  XXXX  XX  XX  XXXX  XX\n"
                << "XXXXXXX   XXXXX  XXXX  XX  XXXX  XXXXX  XXXXXXXXX  XXXXX  XXXX  XXX  X  XXXX  XX\n"
                << "XXXXXXX   XXXXX  XXXX  XX  XXXX  XXXXX  XXX   XXX  XXXXX  XXXX  XXXX    XXXXX XX\n"
                << "XXXXXXX   XXXXX  XXXX  XX  XXXX  XXXXXX  X  X  X  XXXXXX  XXXX  XXXXX   XXXXXXXX\n"
                << "XXXXXXX   XXXXX        XX        XXXXXXX   XXX   XXXXX      XX  XXXXXX  XXXX  XX\n"
                << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
                << "\n" << setw(51) << "You     vs      Hal\n"
                << setw(34) << getPlayer(0)->getPoints() << setw(18-sizeComp.size()) << getPlayer(1)->getPoints() << endl << endl;
    }
    else{ cout <<  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
                << "XXXXXXXXX           XX           XX         XX    X    XXXXXXXXXXXXXXXXXXXXXXXXX\n"
                << "XXXXXXXXXXXXX   XXXXXXXXXX   XXXXXX   XXXXXXXXX   XX   XXXXXXXXXXXXXXXXXXXXXXXXX\n"
                << "XXXXXXXXXXXXX   XXXXXXXXXX   XXXXXX   XXXXXXXXX   XX   XXXXXXXXXXXXXXXXXXXXXXXXX\n"
                << "XXXXXXXXXXXXX   XXXXXXXXXX   XXXXXX      XXXXXXX  XXX  XXXXXXXXXXXXXXXXXXXXXXXXX\n"
                << "XXXXXXXXXXXXX   XXXXXXXXXX   XXXXXX   XXXXXXXXXX  XXX  XXXXXXXXXXXXXXXXXXXXXXXXX\n"
                << "XXXXXXXXXXXXX   XXXXXXXXXX   XXXXXX   XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
                << "XXXXXXXXXXXXX   XXXXXX           XX         XXX   XX   XXXXXXXXXXXXXXXXXXXXXXXXX\n"
                << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n"
                << "\n" << setw(51) << "You     vs      Hal\n"
                << setw(34) << getPlayer(0)->getPoints() << setw(18-sizeComp.size()) << getPlayer(1)->getPoints() << endl << endl;
    }
    for(int i = 0; i<10;i++){
       cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n";
    }
exit(0);

}
/* *********************************************************************
Function Name: eraseStack
Purpose: erase stack from layout
Parameters: int col
Return Value: Void
Local Variables: None
         
Algorithm:
            1) call erase() passing col

Assistance Received: none
********************************************************************* */
void GameBoard::eraseStack(int col){
    layout.erase(layout.begin() + col); 
}
/* *********************************************************************
Function Name: getDebug
Purpose: getter for debugMode
Parameters: None
Return Value: bool
Local Variables: None
         
Algorithm:
            1) return debugMode

Assistance Received: none
********************************************************************* */
bool GameBoard::getDebug(){
    return debugMode;
}
/* *********************************************************************
Function Name: getGameType
Purpose: getter for gameType
Parameters: None
Return Value: Void
Local Variables: None
         
Algorithm:
            1) return gameType

Assistance Received: none
********************************************************************* */
GameBoard::gameMode GameBoard::getGameType(){
    return gameType;
}
/* *********************************************************************
Function Name: getLayoutCard
Purpose: Getter for Layout Card
Parameters: int col, int row
Return Value: Void
Local Variables: None
         
Algorithm:
            1) return layout[col][row]

Assistance Received: none
********************************************************************* */
string GameBoard::getLayoutCard(int col, int row){
    return layout[col][row];
}
/* *********************************************************************
Function Name: getPlayer
Purpose: Get pointer for player objects
Parameters: int i
Return Value: Player*
Local Variables:
         
Algorithm:
            1) return players[i]

Assistance Received: none
********************************************************************* */
Player* GameBoard::getPlayer(int i){
    return players[i];
}
/* *********************************************************************
Function Name: getTurn
Purpose: Getter for playersTurn
Parameters: None
Return Value: string
Local Variables: None
         
Algorithm:
            1) return playersTurn

Assistance Received: none
********************************************************************* */
string GameBoard::getTurn(){
    return playersTurn;
}
/* *********************************************************************
Function Name: layoutClear
Purpose: Clear layout vector
Parameters: None
Return Value: Void
Local Variables: None
         
Algorithm:
            1) Call clear() function

Assistance Received: none
********************************************************************* */
void GameBoard::layoutClear(){
    
    layout.clear();
}
/* *********************************************************************
Function Name: layoutSize
Purpose: Getter for layout size.
Parameters: None
Return Value: int
Local Variables: None
         
Algorithm:
            1) return layout.size()

Assistance Received: none
********************************************************************* */
int GameBoard::layoutSize(){
    return layout.size();
}
/* *********************************************************************
Function Name: layoutStackSize
Purpose:  getter for layout at i size
Parameters: int i
Return Value: int
Local Variables: None
         
Algorithm:
            1) return layout[i].size() 

Assistance Received: none
********************************************************************* */
int GameBoard::layoutStackSize(int i){
    return layout[i].size();
}
/* *********************************************************************
Function Name: loadGame
Purpose: 
Parameters: None
Return Value: Void
Local Variables:
                line, string holding last line
                inFile, ifstream holding file to be loaded
                fileName, string to hold user inputed file name
                doubleDecker, Deck pointer holding new deck

Algorithm:
            1) Prompt user for file name.
            2) Open file and verify
            3) Read file line by line
            4) Process line
             4a) Check word ofline to decide on how to process
             4b) Remove substring identifying process             
             4c) Load line into stringstream one by one
             4d) Save in program one by one

Assistance Received: none
********************************************************************* */
void GameBoard::loadGame(){
    
    string line;
    ifstream inFile;
    string fileName;

    cout << "What file would you like to load? ";
    cin >> fileName;
    cin.ignore();
    inFile.open(fileName);
    if (!inFile) {
        cout << "Unable to open file";
        exit(1); 
    }
    else{cout << "file found and loaded\n"; }

    Deck* doubleDecker = new Deck(0);

    while( getline( inFile, line )){
        if (line.find("Round") != std::string::npos) {
            stringstream ss;
            removeSubstrs(line, "Round:");
            ss << line;
            ss >> round;
        }
        if(line.find("Next") != std::string::npos ){
            removeSubstrs(line, "Next Player: ");
            stringstream ss;
            ss << line;
            ss >> playersTurn;
        }
        if(line.find("Computer") != std::string::npos || line.find("Human") != std::string::npos){
            int i;
            if(line.find("Human") != std::string::npos){ i = 0; }
            else{ i = 1; } 
            getline( inFile, line );
            int temp;
            if(line.find("Score") != std::string::npos) {
                    stringstream ss;
                    removeSubstrs(line, "Score:");
                    ss << line;
                    ss >> temp;
                    players[i]->setPoints(temp);

            }
            getline( inFile, line );
            if(line.find("Hand") != std::string::npos) {
                removeSubstrs(line, "Hand:");
                stringstream ss;
                string card;
                ss<<line;
                do{

                    for (card; ss >> card;) {
                        players[i]->addCard(card);
                    }
                    if (ss.fail())
                    {
                        ss.clear();
                        std::string token;
                        ss >> token;
                    }
                }while (!ss.eof());
            }
            getline( inFile, line );
            if(line.find("Capture") != std::string::npos) {
                removeSubstrs(line, "Capture Pile:");
                stringstream ss;
                ss << line;
                char card;
                do{
                    for (card; ss >> card;) {
                        players[i]->addPair(card);
                    }
                    if (ss.fail())
                    {
                        ss.clear();
                        std::string token;
                        ss >> token;
                    }
                }while (!ss.eof());
            }
  
        }
        if(line.find("Layout") != std::string::npos ){
            removeSubstrs(line, "Layout:");
            stringstream ss;
            ss << line;
            string card;
            do{
                for (card; ss >> card;) {
                    vector<string> row{ card };
                    layout.push_back(row);
                }
                if (ss.fail())
                {
                    ss.clear();
                    string token;
                    ss >> token;
                }
            }while (!ss.eof());
        }
        if(line.find("Row 2") != std::string::npos ){
            removeSubstrs(line, "Row 2:");
            stringstream ss;
            ss << line;
            string card;
            int col = 0;
            do{
                for(card; ss >> card;) {
                    if(card != "XX"){
                        layout[col].push_back(card);
                    }
                    col++;
                }
                if (ss.fail())
                {
                    ss.clear();
                    string token;
                    ss >> token;
                }
            }while (!ss.eof());

        }
        if(line.find("Row 3") != std::string::npos ){
            removeSubstrs(line, "Row 3:");
            stringstream ss;
            ss << line;
            string card;
            int col = 0;
            do{
                for(card; ss >> card;) {
                    if(card != "XX"){
                        layout[col].push_back(card);
                    }
                        col++;
                }
                if (ss.fail())
                {
                    ss.clear();
                    string token;
                    ss >> token;
                }
            }while (!ss.eof());

        }
        if(line.find("Stock Pile") != std::string::npos ){
            removeSubstrs(line, "Stock Pile:");
            stringstream ss;
            ss << line;
            string card;
            do{
                for (card; ss >> card;) {
                    Deck::addCard(card);
                }
                if (ss.fail())
                {
                
                    ss.clear();
                    string token;
                    ss >> token;
                }
            }while (!ss.eof());
        }

    }
    gameLoaded = true;
    inFile.close();
    if(round == 0){ playGame(); }
    else{ playRounds(); }
    

}
/* *********************************************************************
Function Name: playGame
Purpose: 
Parameters: None
Return Value: Void
Local Variables:
                Ethel
Algorithm:
            1) Create new Dealer if not loading game
            2) While players hands are not empty
             2a) Disply board
             2b) Alternate players turn
            

Assistance Received: none
********************************************************************* */
void GameBoard::playGame(){

    if(gameLoaded == false){ Ethel = new Dealer(); }
    while(getPlayer(0)->handSize() > 0 && getPlayer(1)->handSize() > 0){
        if(playersTurn=="Human"){
            displayBoard();
            players[0]->move();
            if(getPlayer(1)->handSize()>0){
                displayBoard();
                    players[1]->move(); 
            }    
        }
        else{
            displayBoard();
            players[1]->move();
            if(getPlayer(0)->handSize()>0){
                displayBoard();
                players[0]->move(); 
            }  
        }
    }
    endGame();
    exit(0);

}
/* *********************************************************************
Function Name: playRounds
Purpose: Play the game in rounds mode
Parameters: None
Return Value: Void
Local Variables:
                more, string to hold user input for more games.
                Ethel, dealer object to deal cards
Algorithm:
            1) If game not loaded, create new dealer object
            2) While more != yes
             2a) While players hands are not empty 
             2b) Disply board
             2c) Alternate players turn
            3) Prompt user for more games


Assistance Received: none
********************************************************************* */
void GameBoard::playRounds(){
   
    string more;

    do{
        round++;
        if(gameLoaded == false){ Ethel = new Dealer(); }
        gameLoaded = false;
        displayBoard();
            while(getPlayer(0)->handSize() != 0 || getPlayer(1)->handSize() != 0){
                if(playersTurn=="Human"){
                    displayBoard();
                    players[0]->move();
                    if(getPlayer(1)->handSize()>0){
                        displayBoard();
                         players[1]->move(); 
                    }    
                }
                else{
                    displayBoard();
                    players[1]->move();
                    if(getPlayer(0)->handSize()>0){
                        displayBoard();
                        players[0]->move(); 
                    }  
                }
            } 
        gameLoaded = false;
        cout << "Do you want to play again? Yes or No?";
        cin >> more;
        while(more != "yes" && more != "y" && more != "no" && more != "n") {
            cin.clear();
            cin.ignore();
            cout << "\nNot a valid choice. Input yes or no: ";
            cin >> more;
            cin.ignore();
        }
        if(more == "n"){ more = "no"; }
    }while(more != "no");
    

}
/* *********************************************************************
Function Name: popFromStack
Purpose: remove card from bottom of stack
Parameters: None
Return Value: Void
Local Variables:
         
Algorithm:
            1) call pop_back on layout at col

Assistance Received: none
********************************************************************* */
void GameBoard::popFromStack(int col){
    layout[col].pop_back();
}
/* *********************************************************************
Function Name: printlayout
Purpose: Print layout
Parameters: None
Return Value: Void
Local Variables:
         
Algorithm:
            1) Cycle through row
            2) Cylce through col
            3) display layout at col and row

Assistance Received: none
********************************************************************* */
void GameBoard::printlayout(){
    
    for(int row = 0; row < 3; row++)        
    {
        for(int col = 0; col < layout.size(); col++)
        {   
            if(layout[col].size() <= row) { 
                if(layout[col].at(0).length() == 3){ cout << "    "; }
                else{cout << "   "; }
            }
            else{ cout << layout[col][row] << " "; }
            
        }
        cout << endl;
    }

}
/* *********************************************************************
Function Name: removeSubstrs
Purpose: Remove substring from string
Parameters: string& s, string p
Return Value: Void
Local Variables:
                n, size place holder
Algorithm:
            

Assistance Received: C++ Cookbook
********************************************************************* */
void GameBoard::removeSubstrs(string& s, string p){ 
    
    string::size_type n = p.length();

    for(string::size_type i = s.find(p); i != string::npos; i = s.find(p)){
      s.erase(i, n);
    }
}
/* *********************************************************************
Function Name: saveGame
Purpose: Serialize game and save to file.
Parameters: None
Return Value: Void
Local Variables:
                line, string placehold for lines to be written into file
                myfile, ofstream object to write to file
Algorithm:
            1) open file
            2) Add descriptor and game result to line
            3) Write line to myFile
            4) Close file
            5) Exit program

Assistance Received: none
********************************************************************* */
void GameBoard::saveGame(){

    string line;
    ofstream myfile ("saveFile.txt");
    if (myfile.is_open())
    {

        myfile << "Round: " << round << "\n\n";
        myfile << "Computer:\n";
        myfile << "   Score: " << getPlayer(1)->getPoints() << "\n" ;
        string temp = "   Hand:";
        for(int i = 0; i < getPlayer(1)->handSize(); i++){
            temp = temp + " " + getPlayer(1)->getCard(i);
        }
        myfile << temp << "\n";
        temp = "   Capture Pile:";
        for(int i=0; i < getPlayer(1)->pairSize(); i++){
            temp = temp + " " + getPlayer(1)->getPair(i);
        }
        myfile << temp << "\n\n";

        myfile << "Human:\n";
        myfile << "   Score: " << getPlayer(0)->getPoints() << "\n" ;
        temp = "   Hand:";
        for(int i = 0; i < getPlayer(0)->handSize(); i++){
            temp = temp + " " + getPlayer(0)->getCard(i);
        }
        myfile << temp << "\n";
        temp = "   Capture Pile:";
        for(int i=0; i < getPlayer(0)->pairSize(); i++){
            temp = temp + " " + getPlayer(0)->getPair(i);
        }
        myfile << temp << "\n\n";

        temp = "Layout:";
        for(int i=0; i < layout.size(); i++){
            temp += " " + layout[i][0];

        }
        myfile << temp << "\n";

        temp = "Row 2: ";
        bool stacks = false;
        for(int i=0; i < layout.size(); i++){
            if(layout[i].size() > 1){
                temp += " " + layout[i][1];
                stacks = true;
            }
            else{
                temp += " XX";
            }
        }
        if(stacks==true){
            myfile << temp << "\n";
        }
        else{ myfile << "\n"; }
        stacks = false;
        temp = "Row 3: ";
        stacks = false;
        for(int i=0; i < layout.size(); i++){
            if(layout[i].size() > 2){
                temp += " " + layout[i][2];
                stacks = true;
            }
            else{
                temp += " XX";
            }
        }
        if(stacks==true){
            myfile << temp << "\n\n";

        }

        temp = "Stock Pile:";
        for(int i=0; i < Deck::deckSize(); i++){
            temp = temp + " " + Deck::getDeckCard(i);
        }
        myfile << temp << "\n\n";

        myfile << "Next Player: " << playersTurn;

        myfile.close();
    }
    else {cout << "Unable to open file";}
    cout << "Game saved! Exiting!\n";
    exit(0); 

}
/* *********************************************************************
Function Name: setTurn
Purpose: Setter for playersTurn
Parameters: string turn
Return Value: Void
Local Variables: None
         
Algorithm:
            1) set playersTurn to turn

Assistance Received: none
********************************************************************* */
void GameBoard::setTurn(string turn){
    playersTurn = turn;
}
/* *********************************************************************
Function Name: startDebug
Purpose: Start a hidden feature of debug. Personal debug purposes only.
Parameters: None
Return Value: Void
Local Variables:
         
Algorithm:
            1) Start an infinite loop
             1a) Create new Dealer
             1b) Run through a full game

Assistance Received: none
********************************************************************* */
void GameBoard::startDebug(){       
     
    while(true){
        Ethel = new Dealer();
        while(getPlayer(0)->handSize() > 0 && getPlayer(1)->handSize() > 0){
            displayBoard();
            players[0]->move();
            displayBoard();
            players[1]->move(); 
        }
    }
}