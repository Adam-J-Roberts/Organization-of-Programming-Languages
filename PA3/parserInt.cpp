/* Implementation of Recursive-Descent Parser
 * parse.cpp
 * Programming Assignment 2
 * Spring 2021
*/

#include "parserInt.h"
#include <math.h> 
//the following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */


static int error_count = 0;
static int depth = 0;
map<string, bool> defVar;
map<string, Token> declType;
map<string, Token> SymTable;
map<string, Value> TempsResults; //Container of temporary locations of Value objects for results of expressions, variables values and constance 
queue <Value> * ValQue; 
static bool logicResults = true;


static bool debug = false;
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

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
    
}
void changeColor(){
	switch(depth)
	{
		case 1:
			cout << RED;
			break;
		case 2:
			cout << GREEN;
			break;
		case 3:
			cout << YELLOW;
			break;	
		case 4:
			cout << BLUE;
			break;	
		case 5:
			cout << MAGENTA;
			break;	
		case 6:
			cout << CYAN;
			break;
		case 7:
			cout << BOLDRED;
			break;
		case 8:
			cout << BOLDGREEN;
			break;
		case 9:
			cout << BOLDYELLOW;
			break;	
		case 10:
			cout << BOLDBLUE;
			break;	
		case 11:
			cout << BOLDMAGENTA;
			break;	
		default:
			cout << RESET;
			break;	

	}
}



//Program is: Prog = PROGRAM IDENT {Decl} {Stmt} END PROGRAM IDENT
bool Prog(istream& in, int& line)
{
	if(debug){
		cout << string( depth, ' ' ) << "Start of Prog\n";
	}
	bool dl = false, sl = false;
	LexItem tok = Parser::GetNextToken(in, line);
	
	
	if (tok.GetToken() == PROGRAM) {
		if(debug){
			cout << "tok.GetToken()) == PROGRAM\n";
		}
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == IDENT) {
			if(debug){
				cout << "tok.GetToken() == IDENT: " << tok.GetLexeme() << endl;
			}
			string progName = tok.GetLexeme();
			dl = Decl(in, line);
			if( !dl  )
			{
				if(debug){
					cout << "Decl(in, line) == true\n";
				}

				ParseError(line, "Incorrect Declaration in Program");
				return false;
			}
			sl = Stmt(in, line);
			if( !sl  )
			{
				if(debug){
					cout << "Stmt(in, line) == true\n";
				}
				
				ParseError(line, "Incorrect Statement in program");
				return false;
			}	
			tok = Parser::GetNextToken(in, line);
			
			if (tok.GetToken() == END) {
				if(debug){
					cout << "tok.GetToken()) == END\n";
				}
				tok = Parser::GetNextToken(in, line);
				
				if (tok.GetToken() == PROGRAM) {
					if(debug){
						cout << "tok.GetToken()) == PROGRAM\n";
					}

					tok = Parser::GetNextToken(in, line);
					
					if (tok.GetToken() == IDENT) {
						if(debug){
							cout << "tok.GetToken()) == IDENT\n";
							cout << string( depth, ' ' ) << "End Prog\n";
							cout << RESET;
						}
						if(progName == tok.GetLexeme()){
							return true;
						}
						else{
							ParseError(line, "Incorrect Program Name");
							return false;
						}

					}
					else
					{
						ParseError(line, "Missing Program Name");
						return false;
					}	
				}
				else
				{
					ParseError(line, "Missing PROGRAM at the End");
					return false;
				}	
			}
			else
			{
				ParseError(line, "Missing END of Program");
				return false;
			}	
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	
	return false;
}

//Decl = Type : VarList 
//Type = INTEGER | REAL | CHAR
bool Decl(istream& in, int& line) {
	bool status = false;
	LexItem tok;
	if(debug){
		depth++;
		changeColor();
		cout << string( depth, ' ' ) << "Start of Decl\n";
	}
	
	LexItem t = Parser::GetNextToken(in, line);
	
	if(t == INTEGER || t == REAL || t == CHAR) {
		if(debug){
			cout << string( depth, ' ' )  << "Decl:" << t << endl;
		}
		tok = t;
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == COLON) {
			if(debug){
				cout << string( depth, ' ' )  << "tok.GetToken() == COLON\n";
			}
			//cout << "t before: " << t << endl;
			status = IdList(in, line, t);
			//cout << "Status = " << status << " t after: " << t  << endl;
			if (status)
			{
				if(debug){
					cout << string( depth, ' ' )  << "IdList == true\n";
				}				
				status = Decl(in, line);
				if(debug){
					cout << string( depth, ' ' ) << "End of Decl\n";
					depth--;
					changeColor();
				}
				return status;
			}
		}
		else{
			ParseError(line, "Missing Colon");
			return false;
		}
	}
		
	Parser::PushBackToken(t);
	if(debug){
		cout << string( depth, ' ' ) << "End of Decl\n";
		depth--;
		changeColor();
	}
	return true;
}

bool Stmt(istream& in, int& line) {

	if(debug){
		depth++;
		changeColor();
		cout << string( depth, ' ' ) << "Start of Stmt\n";
	}

	bool status;
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {

	case PRINT:
		status = PrintStmt(in, line);
		if(debug){
			cout << string( depth, ' ' )  << "tok.GetToken() == PRINT\n";
		}
		if(status)
			status = Stmt(in, line);
		break;

	case IF:
		if(debug){
			cout << string( depth, ' ' )  << "tok.GetToken() == IF\n";
		}		
		status = IfStmt(in, line);

		if(status)
			status = Stmt(in, line);
		break;

	case IDENT:
		if(debug){
			cout << string( depth, ' ' )  << "tok.GetToken() == IDENT: " << t.GetLexeme() <<"\n";
		}
		Parser::PushBackToken(t);
        status = AssignStmt(in, line);
		if(status)
			status = Stmt(in, line);
		break;
		
	case READ:
		if(debug){
			cout << string( depth, ' ' )  << "tok.GetToken() == READ\n";
		}	
		status = ReadStmt(in, line);
		
		if(status)
			status = Stmt(in, line);
		break;

	default:
		if(debug){
			cout << string( depth, ' ' )  << "tok.GetToken() == ELSE\n";
		}
		Parser::PushBackToken(t);
		if(debug){
			cout << string( depth, ' ' ) << "End of Stmt\n";
			depth--;
			changeColor();
		}		
		return true;
	}
	if(debug){
		cout << string( depth, ' ' ) << "End of Stmt\n";
		depth--;
		changeColor();
	}
	return status;
}

//PrintStmt:= print, ExpreList 
bool PrintStmt(istream& in, int& line) { 
    
	if(debug){
		depth++;
		changeColor();
		cout << string( depth, ' ' ) << "Start of PrintStmt\n";
	}

	LexItem t;
    /*createanemptyqueueofValueobjects.*/
    ValQue = new queue<Value>;
    if( (t=Parser::GetNextToken(in,line)) != COMA){
        ParseError(line,"Missing a Comma");
        return false;
    }

    bool ex=ExprList(in,line);
    
    if(!ex){
        ParseError(line,"Missing expression after print");
        while( !(*ValQue).empty() ){
            ValQue->pop();
        }
        delete ValQue;
        return false;
    }

    //Evaluate:print out the list of expressions' values
	if(logicResults){
		while( !(*ValQue).empty() ){
        Value nextVal=(*ValQue).front();
        cout<<nextVal;ValQue->pop();
    }
    cout<<endl;
	}
   

	if(debug){
		cout << string( depth, ' ' ) << "End of PrintStmt\n";
		depth--;
		changeColor();
	}
    return ex;
}//PrintStmt


//IfStmt:= if (Expr) then {Stmt} END IF
bool IfStmt(istream& in, int& line) {

	if(debug){
		depth++;
		changeColor();
		cout << string( depth, ' ' ) << "Start of IfStmt\n";
	}
	bool ex=false ; 
	LexItem t;
	Value retVal;
	if( (t=Parser::GetNextToken(in, line)) != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	ex = LogicExpr(in, line, retVal);
	logicResults = retVal.GetBool();

	if(debug){
		cout << "LogicExpr rslts/ex: " << retVal << ex << endl;
	}
	if( !ex ) {
		ParseError(line, "Missing if statement Logic Expression");
		return false;
	}

	if((t=Parser::GetNextToken(in, line)) != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	
	if((t=Parser::GetNextToken(in, line)) != THEN ) {
		
		ParseError(line, "Missing THEN");
		return false;
	}

	bool st = Stmt(in, line);
	if( !st ) {
		ParseError(line, "Missing statement for IF");
		return false;
	}

	
	
	
	if((t=Parser::GetNextToken(in, line)) != END ) {
		
		ParseError(line, "Missing END of IF");
		return false;
	}
	if((t=Parser::GetNextToken(in, line)) != IF ) {
		
		ParseError(line, "Missing IF at End of IF statement");
		return false;
	}

	if(debug){
		cout << string( depth, ' ' ) << "End of IfStmt\n";
		depth--;
		changeColor();
	}

	logicResults = true;
	return true;
}

bool ReadStmt(istream& in, int& line)
{
	if(debug){
		depth++;
		changeColor();
		cout << string( depth, ' ' ) << "Start of ReadStmt\n";
	}

	LexItem t;
	
	if( (t=Parser::GetNextToken(in, line)) != COMA ) {
		
		ParseError(line, "Missing a Comma");
		return false;
	}
	
	bool ex = VarList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing Variable after Read Statement");
		return false;
	}
	
	if(debug){
		cout << string( depth, ' ' ) << "End of ReadStmt\n";
		depth--;
		changeColor();
	}

	return ex;
}
//IdList:= IDENT {,IDENT}

bool IdList(istream& in, int& line, LexItem& tok) {

	if(debug){
		depth++;
		changeColor();
		cout  << string( depth, ' ') << "Start of IDList\n";
	}

	bool status = false;
	string identstr;
	static LexItem tik;
	(tok == COMA) ? tik = tik : tik = tok;
	//cout << tik;
	tok = Parser::GetNextToken(in, line);
	if(tok == IDENT)
	{
		//set IDENT lexeme to the type tok value
		identstr = tok.GetLexeme();
		//cout << "identStr: " << identstr << endl;
		if (!(defVar.find(identstr)->second))
		{
			if(debug){
				cout << string( depth, ' ' )  << identstr << " is defined\n";
			}
			defVar[identstr] = true;
			SymTable[identstr] = tok.GetToken();
			declType[identstr] = tik.GetToken();
			/*if(tik.GetToken() == INTEGER){
				TempsResults[identstr] = Value(0);
			}
			else if(tik.GetToken() == REAL){
				TempsResults[identstr] = Value(0.0f);
			}*/
			//find me
			//Check here type.GetToken()
		}	
		else
		{
			ParseError(line, "Variable Redefinition");
			return false;
		}
		
	}
	else
	{
		ParseError(line, "Missing Variable");
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	
	if (tok == COMA) {
			if(debug){
				cout << string( depth, ' ' )  << "tok.GetToken() == COMA\n";
			}
		status = IdList(in, line, tok);
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		if(debug){
			cout << string( depth, ' ' ) << "End of IDList\n";
			depth--;
			changeColor();
		}
		return true;
	}
	if(debug){
		cout << string( depth, ' ' ) << "End of IDList\n";
		depth--;
		changeColor();
	}
	return status;
}

//VarList
bool VarList(istream& in, int& line)
{
	if(debug){
		depth++;
		changeColor();
		cout << string( depth, ' ' ) << "Start of VarList\n";
	}

	bool status = false;
	string identstr;
	LexItem tok;
	status = Var(in, line, tok);
	
	if(!status)
	{
		ParseError(line, "Missing Variable");
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	
	if (tok == COMA) {
		status = VarList(in, line);
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		if(debug){
			cout << string( depth, ' ' ) << "End of VarList\n";
			depth--;
changeColor();
		}
		return true;
	}
	if(debug){
		cout << string( depth, ' ' ) << "End of VarList\n";
		depth--;
changeColor();
	}
	return status;
}


bool Var(istream& in, int& line, LexItem& tok)
{
	if(debug){
		depth++;
		changeColor();
		cout << string( depth, ' ' ) << "Start of Var\n";
	}
	//called only from the AssignStmt function
	string identstr;
	
	tok = Parser::GetNextToken(in, line);
	
	if (tok == IDENT){
		if(debug){
			cout << string( depth, ' ' )  << "tok.GetToken() == IDENT\n";
		}
		identstr = tok.GetLexeme();
		if (!(defVar.find(identstr)->second))
		{
			ParseError(line, "Undeclared Variable");
			return false;
			
		}
		if(debug){
			cout << string( depth, ' ' ) << "End of Var\n";
			depth--;
			changeColor();
		}
		return true;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if(debug){
		cout << string( depth, ' ' ) << "End of Var\n";
		depth--;
		changeColor();
	}
	return false;
}


bool AssignStmt(istream& in, int& line) {

	if(debug){
		depth++;
		changeColor();
		cout << string( depth, ' ' ) << "Start of AssignStmt\n";
	}

	bool varstatus = false, status = false;
	LexItem t, t1;
	varstatus = Var( in, line, t);
	//cout << t1 << endl;
	string variable = t.GetLexeme();
	if (varstatus){
		string temp = t.GetLexeme();
		t = Parser::GetNextToken(in, line);
		
		if (t == ASSOP){
			if(debug){
				cout << string( depth, ' ' )  << "tok.GetToken() == ASSOP\n";
			}
			Value retVal;
			status = Expr(in, line, retVal);
			
			
			if(declType[temp] == INTEGER && retVal.GetType() == VREAL){
				int rSide = static_cast<int>( retVal.GetReal());
				retVal = Value( rSide );
			}
			//if(TempsResults[temp].GetType() == VINT && retVal.GetReal() != 0.0){

				//cout << "retVal: " << retVal << "\n";
				//cout << retVal.GetInt();
				//int temp2 = static_cast<int>(retVal.GetReal());
				
				//TempsResults[temp].SetInt( temp2 );
				

			//else
				if(logicResults){
					TempsResults[temp] = retVal; 
					//cout << string( depth, ' ' )  <<  "retVal: " << retVal << endl;
				}
				



			if(!status) {
				ParseError(line, "Missing Expression in Assignment Statment");
				return status;
			}
			
		}
		else if(t.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else {
			ParseError(line, "Missing Assignment Operator =");
			return false;
		}
	}
	else {
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	if(debug){
		cout << string( depth, ' ' ) << "End of AssignStmt\n";
		depth--;
		changeColor();
	}

	return status;	
}

bool ExprList(istream& in, int& line) {
	if(debug){
		depth++;
		changeColor();
		cout << string( depth, ' ' ) << "Start of ExprList\n";
	}
	bool status = false;
	Value retVal;

	status = Expr(in, line, retVal);
	//NOPE!
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	if(logicResults){
		ValQue->push(retVal);
	}
	
	/*
	if(retVal.GetType() == VINT){
		if(retVal.GetInt() != 0){
			ValQue->push(retVal);
		}
	}
	else{
		ValQue->push(retVal);
	}
	*/
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMA) {
		status = ExprList(in, line);
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		if(debug){
			cout << string( depth, ' ' ) << "End of ExprList\n";
			depth--;
			changeColor();
		}
		return true;
	}
	if(debug){
		cout << string( depth, ' ' ) << "End of ExprList\n";
		depth--;
		changeColor();
	}
	return status;
}

bool Expr(istream& in, int& line, Value & retVal) {
	if(debug){
		depth++;
		changeColor();
		cout << string( depth, ' ' ) << "Start of Expr\n";
	}
    Value val1, val2;
    bool t1 = Term(in, line, val1);

    LexItem tok;
    
    if( !t1 ) {
		if(debug){
			cout << string( depth, ' ' ) << "End of Expr\n";
			depth--;
			changeColor();
		}
        return false;
    }
    retVal = val1;

    tok = Parser::GetNextToken(in, line);
    if(tok.GetToken() == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    while ( tok == PLUS || tok == MINUS ) {
        t1 = Term(in, line, val2);

        if( !t1 ) 
        {
            ParseError(line, "Missing operand after operator");
            return false;
        }
		//cout << "retVal.type " << retVal.GetType() << endl;
        if(retVal.GetType()==VCHAR || val2.GetType()==VCHAR){
            ParseError(line,"Run-Time Error-Illegal Mixed Type Operands");
            return false;
        }
        else{
            if(tok == PLUS){
                retVal = retVal + val2;
            }
            else if(tok == MINUS){
                retVal = retVal - val2;
            }
        }

        tok=Parser::GetNextToken(in,line);
        if(tok.GetToken() == ERR){
            ParseError(line,"Unrecognized Input Pattern");
            cout<< "(" << tok.GetLexeme() << ")" << endl;
            return false;
        }
    }
    Parser::PushBackToken(tok);
	if(debug){
		cout << string( depth, ' ' ) << "End of ExprList\n";
		depth--;
		changeColor();
	}
    return true;
}

bool Term(istream& in, int& line, Value& retVal) {
	if(debug){
		depth++;
		changeColor();
		cout << string( depth, ' ' ) << "Start of Term\n";
	}

	float temp = 0;
	Value retVal1 , retVal2;
	bool t1 = SFactor(in, line, retVal);
	retVal1 = retVal;
	if(retVal.GetType() == VINT){
		temp = retVal.GetInt();

	}
	else if(retVal.GetType() == VREAL){
		temp = retVal.GetReal();

	}
	/*else if(retVal.GetType() == VCHAR){
		return true;
	}*/
	LexItem tok;
	
	if( !t1 ) {
		if(debug){
			cout << string( depth, ' ' ) << "End of Term\n";
			depth--;
			changeColor();
		}
		return false;
	}
	
	tok	= Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	while ( tok == MULT || tok == DIV  )
	{
		if(debug){
			cout << string( depth, ' ' ) << "tok == " << tok << "\n";
		}


		t1 = SFactor(in, line, retVal);
		retVal2 = retVal;
		/*
		if(retVal1.GetType() != VINT || retVal1.GetType() != VREAL){
			if(retVal2.GetType() != VINT || retVal2.GetType() != VREAL){
				ParseError(line, "Run-Time Error-Illegal Mixed Type Operands");
				//cout << "(" << tok.GetLexeme() << ")" << endl;
				return false;
			}

		}*/

		if(retVal1.GetType() == VCHAR || retVal2.GetType() == VCHAR){
			ParseError(line, "Run-Time Error-Illegal Mixed Type Operands");
			return false;
		}

		if(tok == MULT){
			if(retVal.GetType() == VINT) temp = temp*retVal.GetInt();
			if(retVal.GetType() == VREAL) temp = temp*retVal.GetReal();
		}
		if(tok == DIV){
			if(retVal.GetType() == VINT) temp = retVal.GetInt();
			if(retVal.GetType() == VREAL) temp = retVal.GetReal();
			if(temp == 0){
				ParseError(line, "Run-Time Error-Illegal Division by Zero");
				return false;
			}
		}

		
		if( !t1 ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}
		LexItem tik = tok;
		tok	= Parser::GetNextToken(in, line);
		//cout  << tok;
		if(tik == DIV){
			cout << tok << endl;
		}
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
		
	}
//cout << retVal << endl;
//cout <<  temp << endl;
	if(temp == 0){
	
	}
	else if(ceil(temp) == temp){

		int newTemp = temp;
		retVal = Value(newTemp);
	}
	else{
		retVal = Value(temp);
	}
	
	Parser::PushBackToken(tok);
	//cout << retVal << endl;
	if(debug){
		cout << string( depth, ' ' ) << "End of Term\n";
		depth--;
		changeColor();
	}
	return true;
}


bool SFactor(istream& in, int& line, Value & retVal)
{
	if(debug){
		depth++;
		changeColor();
		cout << string( depth, ' ' ) << "Start of SFactor\n";
	}
	LexItem t = Parser::GetNextToken(in, line);

	bool status;
	int sign = 0;
	if(t == MINUS )
	{
		if(debug){
			cout << string( depth, ' ' ) << "tok.GetToken() == MINUS\n";
		}
		sign = -1;
	}
	else if(t == PLUS)
	{
		if(debug){
			cout << string( depth, ' ' ) << "tok.GetToken() == PLUS\n";
		}		
		sign = 1;
	}
	else
		Parser::PushBackToken(t);

	
	status = Factor(in, line, sign, retVal);
	if(status){
		if(retVal.GetType() == VINT && sign == -1) retVal.SetInt(retVal.GetInt()*sign);

	}
	if(debug){
		cout << string( depth, ' ' ) << "End of SFactor\n";
		depth--;
		changeColor();
	}
	return status;
}
//LogicExpr = Expr (== | <) Expr
bool LogicExpr(istream& in, int& line, Value& retVal)
{
	
	if(debug){
		depth++;
		changeColor();
		cout << string( depth, ' ' ) << "Start of LogicExpr\n";
	}
	
	//int iLSide = 0, iRSide = 0;
	float fLSide = 0.0, fRSide = 0.0;
	ValType left, right;

	bool t1 = Expr(in, line, retVal);
	left = retVal.GetType();
	if(retVal.GetType() == VINT) fLSide = static_cast<float>(retVal.GetInt());
	else if(retVal.GetType() == VREAL) fLSide = retVal.GetReal();

	
	LexItem tok;
	if(debug){
		cout << string( depth, ' ' )  << "t1/retval: "<< t1 << "/" << retVal << endl;
	}
	
	if( !t1 ) {
		if(debug){
			cout << string( depth, ' ' ) << "End of LogicExpr\n";
			depth--;
		changeColor();
		}
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == LTHAN  || tok == EQUAL) 
	{
		t1 = Expr(in, line, retVal);
		right = retVal.GetType();
		if(right == VCHAR || right == VBOOL || left == VCHAR || left == VBOOL){
			retVal = Value(false);
			ParseError(line, "Run-Time Error-Illegal Mixed Type operation");
			return false;
		}
		if(retVal.GetType() == VINT) fRSide = static_cast<float>(retVal.GetInt());
		else if(retVal.GetType() == VREAL) fRSide = retVal.GetReal();

		if( !t1 ) 
		{
			ParseError(line, "Missing expression after relational operator");
			return false;
		}
		else if(tok == LTHAN){
			retVal = Value(fRSide < fLSide);
			//cout << "fLSide: " << fLSide << " fRSide: " << fRSide << endl;
		 	//Fix this... 
			 /*
			 cout << "retval: " << retVal << endl;
			 retVal = new Value(fRSide < fLSide); //retVal = Value(true) :retVal = Value(false);
			 retVal.SetType(VBOOL);
			 cout << "retval.GetBool: " << retVal.GetBool() << endl;
			 cout << "retval.GetType: " << retVal.GetType() << endl;
			cout << "retval: " << retVal << endl;
			cout << "VBOOL: " << VBOOL << endl;
			 cout << "\nHEEEEEEEERE\n";
			 */
		}
		else if(tok == EQUAL){
			(fRSide == fLSide) ? retVal = Value(true) :retVal = Value(false);

		}
		 	

		if(debug){
			cout << string( depth, ' ' ) << "End of LogicExpr\n";
			depth--;
			changeColor();
		}
		return true; 
	}
	Parser::PushBackToken(tok);
	if(debug){
		cout << string( depth, ' ' ) << "End of LogicExpr\n";
		depth--;
		changeColor();
	}
	return true;
}


bool Factor(istream& in, int& line, int sign, Value& retVal) {
	if(debug){
		depth++;
		changeColor();
		cout << string( depth, ' ' ) << "Start of Factor\n";
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	

	if( tok == IDENT ) {
		if(debug){
			cout << string( depth, ' ' )  << "tok.GetLexeme: " << tok.GetLexeme() << endl;
		}
		//cout << "HEEEEEEEEEEEEEEEEEERE " <<TempsResults[tok.GetLexeme()] << endl;
		string lexeme = tok.GetLexeme();
		//cout << TempsResults.count( lexeme ) << endl;

		//cout << defVar[tok.GetLexeme()] << endl;
		if(logicResults)
		{
					if(!TempsResults.count(lexeme))			
		{
			ParseError(line, "Undefined Variable");
			return false;	
		}
			if (!(defVar.find(lexeme)->second) )
			{
				ParseError(line, "Undefined Variable");
				return false;	
			}
		}
		//cout << string( depth, ' ' )  <<  "retVal.type: " << retVal.GetType() << endl;
		retVal = Value(TempsResults[tok.GetLexeme()]);
		//cout << string( depth, ' ' )  <<  "retVal.type: " << retVal.GetType() << endl;
		if(debug){
			cout << string( depth, ' ' ) << "IDENT: " << retVal << endl;
		}
			//check if the var is defined 
		lexeme = tok.GetLexeme();
		if (!(defVar.find(lexeme)->second))
		{
			ParseError(line, "Undefined Variable");
			return false;	
		}
		if(debug){
			cout << string( depth, ' ' ) << "End of Factor\n";
			depth--;
			changeColor();

		}
		return true;
	}
	else if( tok == ICONST ) {
		
		
		retVal = Value(stoi(tok.GetLexeme()));
		if(debug){
			cout << string( depth, ' ' ) << "ICONST: " << retVal << endl;
		}
		if(debug){
			cout << string( depth, ' ' ) << "End of Factor\n";
			depth--;
			changeColor();
		}
		return true;
	}
	else if( tok == SCONST ) {

		retVal = Value(tok.GetLexeme());
		TempsResults[tok.GetLexeme()] = retVal;
		//cout << string( depth, ' ' )  <<  "retVal: " << retVal << endl;

		//ValQue->push(retVal);
		if(debug){
			cout << string( depth, ' ' ) << "SCONST: " << retVal << endl;
		}
		if(debug){
			cout << string( depth, ' ' ) << "End of Factor\n";
			depth--;
			changeColor();
		}
		return true;
	}
	else if( tok == RCONST ) {
		retVal = Value( stof( tok.GetLexeme() ) );
		if(debug){
			cout << string( depth, ' ' ) << "RCONST: " << retVal << endl;
		}
		if(debug){
			cout << string( depth, ' ' ) << "End of Factor\n";
			depth--;
			changeColor();
		}		
		return true;
	}
	else if( tok == LPAREN ) {
		bool ex = Expr(in, line, retVal);
		if( !ex ) {
			ParseError(line, "Missing expression after (");
			return false;
		}
		if( Parser::GetNextToken(in, line) == RPAREN ){
			if(debug){
				cout << string( depth, ' ' ) << "End of Factor\n";
				depth--;
				changeColor();
			}
			return ex;			
		}


		ParseError(line, "Missing ) after expression");
		return false;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	ParseError(line, "Unrecognized input");
	return 0;
}



