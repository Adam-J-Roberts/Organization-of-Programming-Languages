#include "lex.h"
#include <iostream>
#include <map>
#include <sstream>
#include <string> 

using namespace std;


LexItem id_or_kw(const string& lexeme, int linenum){
    Token token;
    if ( kwmap.find(lexeme) == kwmap.end() ) { 
        token = IDENT;
    } else {
        token = kwmap[lexeme];
    }
    return LexItem(token, lexeme, linenum);
};

ostream& operator<<(ostream& out, const LexItem& tok){
    out << tokenPrint[tok.GetToken()];
    if(tok.GetToken() == IDENT || tok.GetToken() == ICONST || tok.GetToken() == SCONST || tok.GetToken() == RCONST || tok.GetToken() == ERR){
        out << "(" << tok.GetLexeme() << ")";
    }
    out << endl;
    return out;
}
LexItem getNextToken(istream& in, int& linenum){

    map<char,string> operators = {
        { '+', "PLUS" },
		{ '-', "MINUS" },
		{ '*', "MULT" },
		{ '/', "DIV" },
		{ '=', "ASSOP" },
		{ '(', "LPAREN" },
		{ ')', "RPAREN" },   
		{ ':', "COLON" },
		{ ',', "COMA" },
		{ '<', "LTHAN" },
    };
    map<string, Token> operatorsmap = {
        {"PLUS", PLUS },
		{ "MINUS", MINUS },
		{ "MULT", MULT },
		{ "DIV", DIV },
		{ "ASSOP", ASSOP },
		{ "LPAREN", LPAREN },
		{ "RPAREN", RPAREN },   
		{ "COLON", COLON },
		{ "COMA", COMA },
		{ "LTHAN", LTHAN },
    };


    string lexeme;
    char ch;
    string mystring;
    if(in.eof()){ 
        return LexItem(DONE, "", linenum);
    }
    
    while(in.get(ch)){
        mystring += ch;
        //comment
        if(ch=='!'){
            while(in.get(ch)){
                if(ch == '\n'){ 
                    linenum++;
                    break;
                }
            }
            continue;
        }
        //operators
        else if(operators.find(ch) != operators.end() ){
            if(ch == '/'){
                char ch1 = in.peek() ;
                if( ch1 == ch ){
                    in.get(ch);
                    return LexItem(CONCAT,"CONCAT",linenum);
                }
                else{
                    return LexItem(DIV, "DIV", linenum);
                }
            }
            else if(ch == '='){
                char ch1 = in.peek() ;
                if( ch1 == ch ){
                    in.get(ch);
                    return LexItem(EQUAL,"EQUAL",linenum);
                }
                else{
                    return LexItem(ASSOP, "ASSOP", linenum);
                }
            }
            else{
   
                return LexItem(operatorsmap[operators[ch]], operators[ch], linenum);     
            }
        }
        //new Line
        else if(ch=='\n'){
            mystring = "";
            linenum++;
            continue;
        }
        //space
        else if(isspace(ch)){
            continue;
        }
        //string 
        else if(ch=='"' || ch=='\''){
            char ch1 = ch;
            string myString;
            while(in.get(ch)){
                if(ch == ch1){
                    break;
                }
                else if(ch == '\n'){ 
                    return LexItem(ERR, ch1+myString, linenum);
                }
                else{
                    myString += ch;
                }
            }
            return LexItem(SCONST, myString, linenum);
        }
        //words
        else if(isalpha(ch)){
            string myString;
            Token thisToken = IDENT;
            myString += toupper(ch);
            while(in.get(ch)){
        //keywords        
                if(kwmap.find(myString) != kwmap.end() ){
                    thisToken = kwmap[myString];
                    in.putback(ch);
                    return LexItem(thisToken, myString, linenum);
                }
        //IDENTIFIERS
                else if(isalnum(ch)){
                    myString += toupper(ch);
                }
                else{
                    in.putback(ch);
                    break;
                }
            }
            return LexItem(thisToken, myString, linenum);
        }
        //number
        else if(isdigit(ch) || ch == '.'){
            string myInt;
            string str;
            Token thisToken = ICONST;
            myInt += ch;
            str += ch;
            if(ch == '.'){thisToken = RCONST;}
            while(in.get(ch)){     
                char ch1 = in.peek() ;
                if(str == "." && !isdigit(ch) ){ 
                    return LexItem(ERR,str+ch,linenum);
                }
        //REALS
                if(ch == '.' && thisToken == ICONST){
                    if(!isdigit(ch1)){ 
                        return LexItem(ERR,str,linenum);
                    }
                    str+=ch;
                    thisToken = RCONST;
                    myInt += ch;
                }
                else if(ch == '.' && thisToken == RCONST){
                    str+=ch;
                    break;
                }
                else if(isdigit(ch)){
                    str+=ch;
                    myInt += ch;
                }
                else{
                    str+=ch;
                    break;
                }
            }
            in.putback(ch);
            return LexItem(thisToken, myInt, linenum);
        }
        //end of line
        else if(in.eof()){ 
            return LexItem(DONE, "", linenum);
        }
        //invalid symbol
        else{
            string str(1, ch);
            return LexItem(ERR, str, linenum);
            }
    }
    return LexItem(DONE, "", linenum); 

}