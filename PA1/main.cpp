#include <iostream>
#include <map>
#include "lex.h"
#include <cctype>
#include <map>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string> 

using namespace std;

void printMap(map<string, int> map);
void printID(map<string, int> map);
void print(int lines, int tokensCount,map<string, int> strings, map<int, int> ints,
    map<double, int> reals, map<string, int> identifiers );

//command line flags
bool intFlag = false, realFlag = false, stringFlag = false, idFlag = false, verboseFlag = false;

int main(int argc, char *argv[]) {

    string fileName;
    ifstream file; 
    int lines = 0;
    int lineNumber = 1;
    LexItem tok;
    //storage maps
    map<string, int> strings;
    map<int, int> ints;
    map<double, int> reals;
    map<string, int> identifiers;
    //LexItem storage
    vector<LexItem> tokens;

	if (argc > 1) {
        
        int files = 0;

        //handle command line arguements
        for(int i=1; i<argc; i++)
        {
            //handle flags
            if(string(argv[i])[0] == '-'){
                if(string(argv[i]) == "-v") { verboseFlag = true; }
                else if(string(argv[i]) == "-iconsts") { intFlag = true; }
                else if(string(argv[i]) == "-rconsts") { realFlag = true; }
                else if(string(argv[i]) == "-sconsts") { stringFlag = true; }
                else if(string(argv[i]) == "-ids") { idFlag = true; }
                else{ 
                    cout << "UNRECOGNIZED FLAG " << string(argv[i]) << endl;
                    return 1;
                }
            }
            //handle file
            else{
                files++;
                fileName = argv[i];
                //handle multiple files
                if(files>1){
                    cout << "ONLY ONE FILE NAME ALLOWED\n";
                    return 1;
                }
            }
        }
	}
    else{
        cout << "No Specified Input File Name.\n";
        return 1;
    }

    file.open(fileName.c_str());
    //File not opened
    if(!file){
        cout << "CANNOT OPEN THE FILE " << fileName << endl;
        return 1;
    }
    //File empty
    else if(file.peek() == ifstream::traits_type::eof() )
    {
        cout << "Lines: " << lines << endl;
        return 0;
    }
    while( (tok = getNextToken(file, lineNumber) ) != DONE && tok != ERR ){
        lines = tok.GetLinenum();
        if(verboseFlag){ cout << tok; }
        tokens.push_back(tok);

        if(tok.GetToken() == SCONST){
            strings[tok.GetLexeme()] = strings[tok.GetLexeme()]+1;
        }
        else if(tok.GetToken() == ICONST){
            ints[stoi(tok.GetLexeme())] = ints[stoi(tok.GetLexeme())]+1;
        }
        else if(tok.GetToken() == RCONST){
            reals[stod(tok.GetLexeme())] = reals[stod(tok.GetLexeme())]+1;

        }
        else if(tok.GetToken() == IDENT){
            identifiers[tok.GetLexeme()] = identifiers[tok.GetLexeme()]+1;
        }
    }
    if(tok == ERR){
        cout << "Error in line " << tok.GetLinenum() << " (" << tok.GetLexeme() << ")\n";
        return 1;
    }

    print(lines, tokens.size() ,strings, ints, reals, identifiers);


    return 0;
}

void printMap(map<string, int> map){
    
    for (const auto& x : map) {
        cout << x.first << "\n";
    }
}
void printID(map<string, int> map){
    auto it = map.begin();
    cout << it->first;

    for(it = next(map.begin(),1); it != map.end(); advance(it,1)){
        cout << ", " << it->first;
    }
    cout << endl;
}
void print(int lines, int tokensCount,map<string, int> strings, map<int, int> ints,
    map<double, int> reals, map<string, int> identifiers ){
        cout << "Lines: " << lines << endl;
        cout << "Tokens: " << tokensCount << endl;

        if(stringFlag){
            cout << "STRINGS:\n";
            for (const auto& x : strings) {
                cout << x.first << "\n";
            } 
        }
        
        if(intFlag){
            cout << "INTEGERS:\n";
            for (const auto& x : ints) {
                cout << x.first << "\n";
            }
        }

        if(realFlag){
            cout << "REALS:\n";
            for (const auto& x : reals) {
                cout << x.first << "\n";
            }
        }
        
        if(idFlag){
            cout<< "IDENTIFIERS:\n";
            printID(identifiers);
        }


}
