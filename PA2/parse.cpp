/*
* parse.cpp
*/


#include <cctype>
#include <map>
#include <unistd.h>
#include "parse.h"

using std::map;
using namespace std;

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}
	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}
static int error_count = 0;
map<string, bool> defVar;
map<string, Token> SymTable;

int ErrCount()
{
    return error_count;
}
void ParseError(int line, string msg)
{
	++error_count;
    
}
bool Prog(istream& in, int& line){
    LexItem tok;
    if((tok = Parser::GetNextToken(in, line)) == PROGRAM){
        LexItem tik;
        if((tik = Parser::GetNextToken(in, line)) == IDENT){
            LexItem tik2;
            while((tik2 = Parser::GetNextToken(in, line)) != END && !in.eof()){
                if(tik2 == INTEGER || tik2== REAL || tik2 == CHAR){
                        Parser::PushBackToken(tik2);
                        if(Decl(in, line)){
                            continue;
                        }
                        else{
                            //error_count++;
                            return false;
                        }
                        
                }
                else if(tik2 == PRINT || tik2 == IF || tik2 == READ|| tik2 == IDENT){
                    Parser::PushBackToken(tik2);
                    if(Stmt(in, line)){
                        continue;
                    }
                    else{
            
                        //error_count++;
                        return false;
                    }
                }
                
            }
            if(tik2 == END){
                LexItem tak1;
                if((tak1 = Parser::GetNextToken(in, line)) == PROGRAM){
                    LexItem tak2;
                    if((tak2 = Parser::GetNextToken(in, line)) == IDENT){
                        return true;
                    }
                    else{
                        cout << line << ": Missing Program Name\n";
                        error_count++;
                        return false;
                    }
                }
                else{
                    cout << line << ": Missing PROGRAM at the End\n";
                    error_count++;
                    return false;
                }
            }
            //else if
            else{
                error_count++;
                cout << line << ": Missing END of Program\n";
                return false;
            }
        }  
    }
    return false;
}
bool Stmt(istream& in, int& line){
    LexItem tok = Parser::GetNextToken(in, line);
    bool result = true;
    if(tok == PRINT){
        if(PrintStmt(in, line)){
            return true;
        }
        else{
            cout << line << ": Incorrect Statement in program\n";
                error_count++;
            return false;
        }
    } 
    else if(tok == IF){
        if(IfStmt(in, line)){
            return true;
        }
        else{
            cout << line << ": Incorrect Statement in program\n";
                error_count++;
            return false;
        }
    }
    else if(tok == READ){
        if(ReadStmt(in, line)){
            return true;
        }
        else{
            cout << line << ": Incorrect Statement in program\n";
                error_count++;
            return false;
        }
    }
    else if(tok == IDENT){
        map<string,Token>::iterator it;
        it = SymTable.find(tok.GetLexeme());
        if(it == SymTable.end()){    
            error_count++; 
            cout << line << ": Undeclared Variable\n";
            result=false;
        }    
        if(AssignStmt(in, line)){
                return result;
        }
        else{
                cout << line << ": Incorrect Statement in program\n";
                error_count++;
                return false;
        }
    }
    else{
        Parser::PushBackToken(tok);
        return false;
    }
    
}
bool Decl(istream& in, int& line){
    LexItem tok = Parser::GetNextToken(in, line);
    if(tok == INTEGER || tok == REAL || tok == CHAR){
        LexItem tik;
        if((tik = Parser::GetNextToken(in, line)) == COLON){
            if(IdList(in, line, tok)){
                return true;
            } 
            else{
                cout << line << ": Missing Variable\n";
                error_count++;
                return false;
            }
        }
        else{
            cout << line << ": Incorrect Declaration in Program\n";
            error_count++;
            Parser::PushBackToken(tik);
            return false;
        }    
    }
    else{
        Parser::PushBackToken(tok);
        return false;
    }
}
bool PrintStmt(istream& in, int& line){
    LexItem tok = Parser::GetNextToken(in, line);
    if(tok  == COMA){
        if(ExprList(in, line)){
            return true;
        }
        else{
            cout << line << ": Missing Expression after print\n";
            error_count++;
            return false;
        }
    }
    else{
        cout << line << ": Missing a Comma\n";
        error_count++;
        //Parser::PushBackToken(tok);
        return false;
    }

}
bool IfStmt(istream& in, int& line){
    LexItem tak = Parser::GetNextToken(in, line);
    if(tak == LPAREN){
        
        if(LogicExpr(in, line)){
            LexItem tak2 = Parser::GetNextToken(in, line);
            if(tak2==RPAREN){
                LexItem tik = Parser::GetNextToken(in, line);
                if(tik == THEN){
                    while(Stmt(in, line)){}
                    LexItem took = Parser::GetNextToken(in, line);
                    if(took == END){
                        LexItem took2 = Parser::GetNextToken(in, line);
                        if(took2 == IF){
                            return true;
                        }
                        else{
                            Parser::PushBackToken(took2);
                            return false;
                        }
                    }
                    else{
                        //error_count++;
                        Parser::PushBackToken(took);
                        return false;
                    }
                }
                else{
                    //error_count++;
                    Parser::PushBackToken(tik);
                    return false;
                }
            }
            else{
                cout << line << ": Missing Right Parenthesis\n";
                error_count++;
                Parser::PushBackToken(tak2);
                return false;
            }
        }
        else{
            cout << line << ": Incorrect IF condition\n";
            error_count++;
            return false;
        }
    }
    else{
        //error_count++;
        Parser::PushBackToken(tak);
        return false;
    }
}
bool ReadStmt(istream& in, int& line){
    LexItem tok;
    if((tok = Parser::GetNextToken(in, line)) == COMA){
        if(VarList(in, line)){
            return true;
        }
        else{
            cout << line << ": Missing Variable after Read Statement\n";
            error_count++;
            Parser::PushBackToken(tok);
            return false;
        } 
    }
    else{
        //error_count++;
        Parser::PushBackToken(tok);
        return false;
    }
}
bool IdList(istream& in, int& line, LexItem tok){
    LexItem tak;
    if((tak = Parser::GetNextToken(in, line)) == IDENT){
        map<string,Token>::iterator it;
        it = SymTable.find(tak.GetLexeme());
        if(it == SymTable.end()){
            SymTable.insert({tak.GetLexeme(), tok.GetToken()});
            LexItem tak1;
            if((tak1 = Parser::GetNextToken(in, line)) == COMA){
                if(IdList(in, line, tok)) return true;
                else{
                    //error_count++;
                    return false;
                }
            }
            else{
                Parser::PushBackToken(tak1);
                return true;
            }
        }
        else{
            error_count++;
            cout << line << ": Variable Redefinition\n";
            return false;
        }
    }
    else{
        //error_count++;
        Parser::PushBackToken(tok);
        return false;
    }
}
bool VarList(istream& in, int& line){
    if(Var(in, line)){
        LexItem tok;
        if((tok = Parser::GetNextToken(in, line)) == COMA){
            if(VarList(in, line)){
                return true;
            }
            else{
                cout << line << ": Missing Variable\n";
                error_count++;
                Parser::PushBackToken(tok);
                return false;
            }
        }
        else{
            Parser::PushBackToken(tok);
            return true;
        }
    }
    else{
        //error_count++;
        //Parser::PushBackToken(tok);
        return false;
    }
}
bool Var(istream& in, int& line){
    LexItem tok = Parser::GetNextToken(in, line);
     if(tok==IDENT){
            map<string,Token>::iterator it;
            it = SymTable.find(tok.GetLexeme());
            if(it != SymTable.end()){
                return true;
            }
            else{
                cout << line << ": Undeclared Variable\n";
                error_count++;
                return false;
            }
        }
    else{
        cout << line << ": Undeclared Variable\n";
        error_count++;//maybe
        Parser::PushBackToken(tok);
        return false;
    }




}
bool AssignStmt(istream& in, int& line){
        LexItem tok = Parser::GetNextToken(in, line);
        if(tok == ASSOP){
            if(Expr(in, line)){
                return true;
            }    
            else{
                //error_count++;
                Parser::PushBackToken(tok);
                return false;
            }
        }
        else{
            //error_count++;
            Parser::PushBackToken(tok);
            return false;
        }


}
bool ExprList(istream& in, int& line){
    if(Expr(in, line)){
        LexItem tok = Parser::GetNextToken(in, line);
        if(tok  == COMA){
            if(ExprList(in, line)){
                return true;
            }
            else{
                //error_count++;
                return false;
            } 
        }
        else{
            Parser::PushBackToken(tok);
            return true;
        }   
    }
    else{
        cout << line << ": Missing Expression\n";
        error_count++;
        return false;
    }
}
bool LogicExpr(istream& in, int& line){
    if(Expr(in, line)){
        LexItem tok = Parser::GetNextToken(in, line);

        if(tok == LTHAN || tok == EQUAL){
         
            if(Expr(in, line)){
                return true;
            }
            else{
                //error_count++;
                Parser::PushBackToken(tok);
                return false;
            }
        }
        else{
            //error_count++;
            Parser::PushBackToken(tok);
            return false;
        }
    }
    else{
        cout << "Here\n";
        //error_count++;
        return false;
    }
}
bool Expr(istream& in, int& line){
    if(Term(in, line)){
        LexItem tok = Parser::GetNextToken(in, line);
        if(tok == PLUS || tok == MINUS){
            if(Term(in, line)){
                return true;
            }
            else{
                //error_count++;
                Parser::PushBackToken(tok);
                return false;
            }
        }
        else{
            Parser::PushBackToken(tok);
            return true;
        }
    }
    else{
        //error_count++;
        return false;
        //error
    }
}
bool Term(istream& in, int& line){
    if(SFactor(in, line)){
        LexItem tok = Parser::GetNextToken(in, line);
        if(tok == MULT || tok == DIV){
            if(Term(in, line)){
                return true;
            }
            else{
                //error_count++;
                return false;
            }
        }
        else{
            Parser::PushBackToken(tok);
            return true;
        }
    }
    else{
        //error_count++;
        return false;
        //error
    }
    return true;
}
bool SFactor(istream& in, int& line){
    LexItem tok= Parser::GetNextToken(in, line);
    if(tok == PLUS || tok == MINUS){
        if(Factor(in, line, 0)){
            return true;
        }
        else{
            //error
            return false;
        }
    }
    else if(tok ==ERR){
        error_count++;
        cout << line << ": Unrecognized Input Pattern\n(" << tok.GetLexeme() << ")\n";
        return false;
    }
    //HERE!!!
    else{
        Parser::PushBackToken(tok);
        if(Factor(in, line, 0)){
            return true;
        }
        else{
            //error_count++;
            return false;
        }
        
    }
}
bool Factor(istream& in, int& line, int sign){
    LexItem tok = Parser::GetNextToken(in, line);
    if(tok == IDENT || tok == ICONST || tok == RCONST || tok == SCONST || tok== LPAREN){
        if(tok == LPAREN){
            if(Expr(in, line)){
                LexItem tik = Parser::GetNextToken(in, line);
                if(tik == RPAREN){
                    return true;
                }
                else{
                    //error_count++;
                    Parser::PushBackToken(tik);
                    return false;
                }
            }
            else{
                //error_count++;
                return false;
            }
        }
        else if(tok==IDENT){
            map<string,Token>::iterator it;
            it = SymTable.find(tok.GetLexeme());
            if(it != SymTable.end()){
                return true;
            }
            else{
                //error_count++;
                return false;
            }
        }

        else{
            return true;
        }
    }
    else{
        Parser::PushBackToken(tok);
        return false;
    }
}