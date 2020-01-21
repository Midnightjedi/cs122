#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <climits>
using namespace std;

bool isValid;
int i;
int j;
int charsTranslated;
int count = 0;
vector<char> english = {'a', 'b', 'c'};
vector<string> morse = {"b"};

char GetInput(string &inputFile, string &outputFile);
void Translate(ifstream&, string fileName, string &text, char choice);
void TextToMorse(ifstream&, string &text);
void MorseToText(ifstream&, string &text);
string TranslateCharToMorse(char in);
char TranslateMorseToChar(string in);

// main()
int main(){
    string inputFile;
    string outputFile;
    string outputText;
    char choice = GetInput(inputFile, outputFile);

    if(choice == 'm'){
        cout << "\nConverting to morse...\n";
    } else if(choice == 't'){
        cout << "\nConverting to english...\n";
    } else{
        cout << "\nError!\n";
        exit(-1);
    }

    ifstream inFile;
    ofstream outFile;

    outFile.open(outputFile);
    Translate(inFile, inputFile, outputText, choice);
    outFile << outputText;
    outFile.close();

    cout << outputText << endl;

    return 0;
}
//===============================================================================



//Gets user command line input
char GetInput(string &inputFile, string &outputFile){
    string input;
    char output;

    cout << "[Prompt]: ";
    cin >> input;
    if(input == "-m"){
        output = 'm';
    } else if(input == "-t"){
        output = 't';
    } else{
        cout << "\nInvalid argument(s)!\n";
        exit(-1);
    }

    cin >> input;
    inputFile = input;
    cout << input << endl;
    cin >> input;
    outputFile = input;
    cout << input << endl;

    return output;
}
//===============================================================================



//Handles opening file
void Translate(ifstream& inFile, string fileName, string &text, char choice){

    inFile.open(fileName);
    if(!inFile.is_open()){
        cout << "Failed to open file!\n";
        exit(-1);
    } else{
        cout << "File opened\n";
    }

    if(choice == 'm'){
        TextToMorse(inFile, text);
    } else if(choice == 't'){
        MorseToText(inFile, text);
    } else{
        cout << "\nError!\n";
        exit(-1);
    }
    cout << "Closing file...\n";
    inFile.close();
}
//===============================================================================



//Translates text to morse and sends resulting string back to OpenFile()
void TextToMorse(ifstream& inFile, string &text){
    string line;
    string word;
    string morse;
    istringstream iss;

    while(!inFile.eof()){
        getline(inFile, line);
        if(inFile.good()){
            iss.clear();
            iss.str(line);
            while(iss.good()){
                iss >> word;
                for(char c: word){
                    morse = TranslateCharToMorse(c);
                    text = text + morse + ' ';
                }
                text = text + ' ';
            }
            text = text + '\n';
        }
    }
}
//===============================================================================


//Translates morse to text and sends resulting string back to OpenFile()
void MorseToText(ifstream& inFile, string &text){
    string line;
    string morseWord;
    string morseChar;
    string inputText;
    char character;
    vector<int> indexDoubles;
    vector<int> indexSingles;
    indexDoubles.push_back(0);
    indexSingles.push_back(0);

    while(!inFile.eof()){
        count++;
        if(count > 10000){
            cout << "Loop took too long!\n[Line parsing]\n";
            exit(-1);
        }
        getline(inFile, line);
        if(inFile.good()){
            cout << "    [Got line from file]:\n        |" << line << "|" << endl;
            if(line.substr(line.size()-2, line.size()-1) == "  "){
                line.erase(line.size()-2, line.size()-1);
            }
            if(line.substr(0,1) == "  "){
                line.erase(0,1);
            }
            cout << "    [Line cleaned up]:\n        |" << line << "|" << endl;
            //Removes any double spaces at start and end of each line

            indexDoubles = {0};
            isValid = true;
            while(isValid == true){
                if(line.find("  ", indexDoubles.at(indexDoubles.size()-1)+1) != std::string::npos){
                    indexDoubles.push_back(line.find("  ", indexDoubles.at(indexDoubles.size()-1)+1));
                    count++;
                } else{
                    isValid = false;
                    indexDoubles.push_back(line.size()-1);
                }
                count++;
                if(count > 10000){
                    cout << "Loop took too long![Double space indexing]\n";
                    exit(-1);
                }
            }
            cout << "    [Double spaces indexed]:\n        ";
            for(i = 0; i < indexDoubles.size(); i++){
                cout << indexDoubles.at(i) << ' ';
            }
            cout << endl;
            //Indexes location of double spaces
            for(j = 0; j < indexDoubles.size()-1; j++){
                cout << " j = " << j << endl;
                cout << line << endl;
                if(j == 0){
                    morseWord = line.substr(indexDoubles.at(j), indexDoubles.at(j+1)-indexDoubles.at(j));
                } else if(j == indexDoubles.size()-2){
                    morseWord = line.substr(indexDoubles.at(j)+2, line.size()-indexDoubles.at(j)+2);
                } else{
                    morseWord = line.substr(indexDoubles.at(j)+2, indexDoubles.at(j+1)-(indexDoubles.at(j)+2));
                }
                if(indexDoubles.size() == 2){
                    morseWord = line.substr(indexDoubles.at(j), indexDoubles.at(j+1)-indexDoubles.at(j)+1);
                }
                cout << "    [Cut out morse word]:\n        |" << morseWord << "|" << endl;
                //cuts into morse words
                
                if(morseWord.at(morseWord.size()-1) == ' '){
                    morseWord.erase(morseWord.size()-2);
                    cout << "cleaning space\n";
                }
                if(morseWord.at(0) == ' '){
                    morseWord.erase(0);
                    cout << "cleaning space\n";
                }

                indexSingles = {0};
                isValid = true;
                while(isValid == true){
                    if(morseWord.find_first_of(' ', indexSingles.at(indexSingles.size()-1)+1) != std::string::npos){
                        indexSingles.push_back(morseWord.find_first_of(' ', indexSingles.at(indexSingles.size()-1)+1));
                        cout << "    [Found space]\n";
                    } else{
                        isValid = false;
                        indexSingles.push_back(morseWord.size()-1);
                    }
                    count++;
                    if(count > 10000){
                        cout << "Loop took too long!\n[Single space indexing]\n";
                        exit(-1);
                    }
                }
                cout << "    [Single spaces indexed]:\n        ";
                for(i = 0; i < indexSingles.size(); i++){
                    cout << indexSingles.at(i) << ' ';
                }
                cout << endl;
                //Indexes spaces
                for(i = 0; i < indexSingles.size()-1; i++){
                    
                    if(i == 0){
                        morseChar = morseWord.substr(indexSingles.at(i), indexSingles.at(i+1));
                    } else if(i == indexSingles.size()-2){
                        morseChar = morseWord.substr(indexSingles.at(i)+1);
                    } else{
                        morseChar = morseWord.substr(indexSingles.at(i)+1, indexSingles.at(i+1)-(indexSingles.at(i)+1));
                    }
                    if(indexSingles.size() == 2){
                        morseChar = morseWord.substr(indexSingles.at(i), indexSingles.at(i+1)-indexSingles.at(i)+1);
                    }
                    cout << "    [Cut out morse character]:\n        |" << morseChar << "| i = " << i << endl;
                    text = text + TranslateMorseToChar(morseChar);
                    count++;
                    if(count > 10000){
                        cout << "Loop took too long!\n[Character parsing]\n";
                        exit(-1);
                    }
                }
                //Cuts into morse characters, translates to text, and appends to string "text"
                text = text + ' ';
                count++;
                if(count > 10000){
                    cout << "Loop took too long!\n[Word parsing]\n";
                    exit(-1);
                }
            }
            text.erase(text.size()-1);
            text = text + '\n';
            //Cuts, indexes, and translates morse from lines

        }
    }
}
//===============================================================================



//Translates morse string to individual character
char TranslateMorseToChar(string in){
    char out;

        if(in == ".-"){
            out = 'A';
            
        } else if(in == "-..."){
            out = 'B';
            
        } else if(in == "-.-."){
            out = 'C';
            
        } else if(in == "-.."){
            out = 'D';
            
        } else if(in == "."){
            out = 'E';
            
        } else if(in == "..-."){
            out = 'F';
            
        } else if(in == "--."){
            out = 'G';
            
        } else if(in == "...."){
            out = 'H';
            
        } else if(in == ".."){
            out = 'I';
            
        } else if(in == ".---"){
            out = 'J';
            
        } else if(in == "-.-"){
            out = 'K';
            
        } else if(in == ".-.."){
            out = 'L';
            
        } else if(in == "--"){
            out = 'M';
            
        } else if(in == "-."){
            out = 'N';
            
        } else if(in == "---"){
            out = 'O';
            
        } else if(in == ".--."){
            out = 'P';
            
        } else if(in == "--.-"){
            out = 'Q';
            
        } else if(in == ".-."){
            out = 'R';
            
        } else if(in == "..."){
            out = 'S';
            
        } else if(in == "-"){
            out = 'T';
            
        } else if(in == "..-"){
            out = 'U';
            
        } else if(in == "...-"){
            out = 'V';
            
        } else if(in == ".--"){
            out = 'W';
            
        } else if(in == "-..-"){
            out = 'X';
            
        } else if(in == "-.--"){
            out = 'Y';
            
        } else if(in == "--.."){
            out = 'Z';

        } else if(in == "-----"){
            out = '0';

        } else if(in == ".----"){
            out = '1';

        } else if(in == "..---"){
            out = '2';

        } else if(in == "...--"){
            out = '3';

        } else if(in == "....-"){
            out = '4';

        } else if(in == "....."){
            out = '5';

        } else if(in == "-...."){
            out = '6';

        } else if(in == "--..."){
            out = '7';

        } else if(in == "---.."){
            out = '8';

        } else if(in == "----."){
            out = '9';

        } else{
            out = '#';
        }
    charsTranslated++;
    cout << "    [Character translated]:\n        " << out << endl;
    return out;
}
//===============================================================================



//Translates individual character to morse
string TranslateCharToMorse(char in){
    string out;

    switch(in){
        case 'a':
        case 'A':
            out = ".-";
            break;
        case 'b':
        case 'B':
            out = "-...";
            break;
        case 'c':
        case 'C':
            out = "-.-.";
            break;
        case 'd':
        case 'D':
            out = "-..";
            break;
        case 'e':
        case 'E':
            out = ".";
            break;
        case 'f':
        case 'F':
            out = "..-.";
            break;
        case 'g':
        case 'G':
            out = "--.";
            break;
        case 'h':
        case 'H':
            out = "....";
            break;
        case 'i':
        case 'I':
            out = "..";
            break;
        case 'j':
        case 'J':
            out = ".---";
            break;
        case 'k':
        case 'K':
            out = "-.-";
            break;
        case 'l':
        case 'L':
            out = ".-..";
            break;
        case 'm':
        case 'M':
            out = "--";
            break;
        case 'n':
        case 'N':
            out = "-.";
            break;
        case 'o':
        case 'O':
            out = "---";
            break;
        case 'p':
        case 'P':
            out = ".--.";
            break;
        case 'q':
        case 'Q':
            out = "--.-";
            break;
        case 'r':
        case 'R':
            out = ".-.";
            break;
        case 's':
        case 'S':
            out = "...";
            break;
        case 't':
        case 'T':
            out = "-";
            break;
        case 'u':
        case 'U':
            out = "..-";
            break;
        case 'v':
        case 'V':
            out = "...-";
            break;
        case 'w':
        case 'W':
            out = ".--";
            break;
        case 'x':
        case 'X':
            out = "-..-";
            break;
        case 'y':
        case 'Y':
            out = "-.--";
            break;
        case 'z':
        case 'Z':
            out = "--..";
            break;
        default:
            out = "#";
            break;
    }
    charsTranslated++;
    return out;
}
//===============================================================================