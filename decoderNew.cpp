/*
Name: Maxwell Nygren
Class: CPSC122, Spring 2020
Modified: 1-22-20
Assignment: PA1
Description: Takes a .txt file of morse or english and translates it to the opposite
             code/language, then writes the result to an output file based on user input.
Notes: cplusplus.com for syntax on certain functions
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <climits>
using namespace std;

bool isValid;
int charsTranslated;
int count = 0;
char english[123];
const string morse[91] = {"","","","","","","","","","","","","","","","","","","","",
            "","","","","","","","","","","","","","","","","","","","",
            "","","","","","","","","-----",".----","..---","...--","....-",".....","-....","--...","---..","----.","","",
            "","","","","",".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",
            ".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."};

char GetInput(string &inputFile, string &outputFile);
void Translate(ifstream&, string fileName, string &text, char choice);
void TextToMorse(ifstream&, string &text);
void MorseToText(ifstream&, string &text);
string TranslateCharToMorse(char in);
char TranslateMorseToChar(string in);

// main()
int main(){
    int i;
    string inputFile;
    string outputFile;
    string outputText;
    char choice = GetInput(inputFile, outputFile);

    for(i = 0; i < 123; i++){
        english[i] = static_cast<char>(i);
    }

    for(i = 0; i < 91; i++){
        cout << english[i] << ' ' << morse[i] << endl;
    }

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


    Translate(inFile, inputFile, outputText, choice);

    outFile.open(outputFile);
        if(outFile.is_open()){
            outFile << outputText;
            outFile.close();
        } else{
            cout << "Failed to open/create output file!\n";
            exit(-1);
        }

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



//Opens input file based on user inputs and calls relevant translation function
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
    int i;
    int j;
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
            //Gets line from file

            while(1){
                if(line.at(line.size()-1) == ' '){
                    line.erase(line.size()-1);
                } else{
                    break;
                }
            }
            while(1){
                if(line.at(0) == ' '){
                    line.erase(0);
                } else{
                    break;
                }
            }
            cout << "    [Line cleaned up]:\n        |" << line << "|" << endl;
            //Removes any spaces at start and end of each line

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
            //Indexes location of double spaces (finds words)

            for(j = 0; j < indexDoubles.size()-1; j++){
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
                //Cuts into morse words using double space index
                
                if(morseWord.at(morseWord.size()-1) == ' '){
                    morseWord.erase(morseWord.size()-2);
                    cout << "cleaning space\n";
                }
                if(morseWord.at(0) == ' '){
                    morseWord.erase(0);
                    cout << "cleaning space\n";
                }
                //Failsafe to remove spaces at start and end

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
                    cout << "    [Cut out morse character]:\n        |" << morseChar << "|\n";
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
    int i;

    for(i = 0; i < 91; i++){
        if(morse[i] == in){
            out = english[i];
            break;
        }
    }

    charsTranslated++;
    cout << "    [Character translated]:\n        " << out << endl;
    return out;
}
//===============================================================================



//Translates individual character to morse
string TranslateCharToMorse(char in){
    string out;

    if(static_cast<int>(in) <= 96){
        out = morse[static_cast<int>(in)];
    } else{
        out = morse[static_cast<int>(in)-32];
    }

    charsTranslated++;
    return out;
}
//===============================================================================