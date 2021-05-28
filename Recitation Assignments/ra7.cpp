#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>

using namespace std;

bool inLanguage(string& inputString);

int main(int argc, char *argv [])
{
	string sentence;
	bool isInLanguage;
	int Lcount=0;
	
	ifstream file;
	if( argc >= 2 ) {
		file.open(argv[1]);
		if( !file) 
		{
			/* error */ 
			cerr << "File cannot be opened: " << argv[1] << endl;
			exit (1);
        }
				
	}
	else
	{
		cerr << "No file is found" << endl;
		exit (1);
	}
	
	getline(file, sentence);
       
    while ( !file.eof()) {
       	Lcount++;
       	isInLanguage = inLanguage(sentence);
       	if (isInLanguage)
       	{
       		cout << sentence << " is a sentence in the Language." << endl;
		}
		else
		{
			cout << sentence << " is not a sentence in the Language." << endl;
		}
		 	
		getline(file, sentence); 
	}
}
bool inLanguage(string& inputString){
    stack<char> reverse;
    queue<char> forward;
    bool flip = false;
    char deez, nutz;

    if(inputString.size() < 1) {
        cout << "Empty string is not in the Language.\n";
        exit(0);
        return false;
    }

    for(int i = 0; i != inputString.length(); i++){
        if(inputString[i] == '$') flip = true;
        else if(flip == true) reverse.push(inputString[i]);
        else forward.push(inputString[i]);
    }
    if(flip != true){
        cout << "string is missing $.\n";
        return false;
    }
    else if(forward.size() == reverse.size()){
        for(int j = 0; j < forward.size(); j++){
            if(forward.front() != reverse.top() ){
                return false;
            }
            else{
                forward.pop();
                reverse.pop();
            }
        }
    }
    else{
        return false;
    }

    return flip;
}
