/*=====================================
Christopher Lombardi 
CS 280 
09/24/24
RA 4
=======================================*/

#include <iostream>
#include <cstdlib>
#include <cctype>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <map>
#include <set>

using namespace std;

int main(int argc, char* argv[]){
    string filename;

    if(argc < 2){
        cout << "NO SPECIFIED INPUT FILE NAME." << endl;
        exit(1);
    } 
    else{
        filename = argv[1];
    }

    ifstream file(filename);

    if (!file.is_open()) {
        cout << "CANNOT OPEN THE FILE " << filename << endl;
        exit(1);
    }

    set<string> valid_options = {"-all", "-bin", "-dec", "-hex", "-oct"};
    set<string> options;


    for(int i = 2; i < argc; i++) {
        string arg = argv[i];
        if(valid_options.find(arg) != valid_options.end()){
            options.insert(arg);  // Add recognized option to the set
        } else {
            cout << "UNRECOGNIZED FLAG " << arg << endl;
            exit(1);  // Print unrecognized flag
        }
    }

    // Valid literal patterns
    auto const decimal = regex("[1-9][0-9]+");
    auto const hexa = regex("^0(x|X)[0-9a-fA-F]+$");
    auto const octa = regex("^0[0-7]+$");
    auto const binary = regex("^0(b|B)[01]+$");

    // Invalid literal patterns
    auto const invalid_hexa = regex("^0(x|X)[0-9a-fA-F]+[g-zG-Z*&^%$#@!].|0(x|X)[0-9a-fA-F]*[^0-9a-fA-F]");
    auto const invalid_octa = regex("^0[0-7]*[*&^%$#@!8-9].");
    auto const invalid_binary = regex("^0(b|B)[0-1]+[*&^%$#@!].|^0(b|B)[2-9]+");
    auto const invalid_decimal = regex("^[1-9]+[a-zA-Z*&^%$#!+()]+[0-9]*|^[0-9]+\\.[0-9]+");

    int numLines = 0;
    int numWords = 0;
    int numDecimals = 0;
    int numHexa = 0;
    int numOctal = 0;
    int numBinary = 0;

    map<string, int> binary_literals;
    map<string, int> hexadecimal_literals;
    map<string, int> decimal_literals;
    map<string, int> octal_literals;

    string line;

    while(getline(file, line)){
        numLines++;
        istringstream iss(line);
        string word;

        while(iss >> word){
            numWords++;

            if (regex_match(word, invalid_binary)) {
                cout << "Incorrect Binary Literal " << word << " at Line: " << numLines << endl;
            } else if (regex_match(word, invalid_hexa)) {
                cout << "Incorrect Hexadecimal Literal " << word << " at Line: " << numLines << endl;
            } else if (regex_match(word, invalid_octa)) {
                cout << "Incorrect Octal Literal " << word << " at Line: " << numLines << endl;
            } else if (regex_match(word, invalid_decimal)) {
                cout << "Incorrect Decimal Literal " << word << " at Line: " << numLines << endl;
            } else if (regex_match(word, decimal)) {
                decimal_literals[word]++;
                numDecimals++;
            } else if (regex_match(word, hexa)) {
                hexadecimal_literals[word]++;
                numHexa++;
            } else if (regex_match(word, octa)) {
                octal_literals[word]++;
                numOctal++;
            } else if (regex_match(word, binary)) {
                binary_literals[word]++;
                numBinary++;
            }
        }
    }

    if(numLines == 0){
        cout << "File is empty." << endl;
    }
    

    cout << "Total Number of Lines: " << numLines << endl;
    cout << "Number of Words: " << numWords << endl;
    cout << "Number of Decimal Literals: " << numDecimals << endl;
    cout << "Number of Hexadecimal Literals: " << numHexa << endl;
    cout << "Number of Octal Literals: " << numOctal << endl;
    cout << "Number of Binary Literals: " << numBinary << endl;

    map<string, int>::iterator it;

    // If "-all" is included, output everything
    if(options.count("-all")) {
        cout << endl;
        cout << "List of Binary-Literals and their number of occurrences:" << endl;
        for(it = binary_literals.begin(); it != binary_literals.end(); it++){
            cout << it->first << ": " << it->second << endl;
        }
        cout << endl;
        cout << "List of Decimal-Literals and their number of occurrences:" << endl;
        for(it = decimal_literals.begin(); it != decimal_literals.end(); it++){
            cout << it->first << ": " << it->second << endl;
        }
        cout << endl;
        cout << "List of Hex-Literals and their number of occurrences:" << endl;
        for(it = hexadecimal_literals.begin(); it != hexadecimal_literals.end(); it++){
            cout << it->first << ": " << it->second << endl;
        }
        cout << endl;
        cout << "List of Octal-Literals and their number of occurrences:" << endl;
        for(it = octal_literals.begin(); it != octal_literals.end(); it++){
            cout << it->first << ": " << it->second << endl;
        }
    } else {
        // Otherwise, check each individual option
        if(options.count("-bin")) {
            cout << endl;
            cout << "List of Binary-Literals and their number of occurrences:" << endl;
            for(it = binary_literals.begin(); it != binary_literals.end(); it++){
                cout << it->first << ": " << it->second << endl;
            }
        }

        if(options.count("-dec")) {
            cout << endl;
            cout << "List of Decimal-Literals and their number of occurrences:" << endl;
            for(it = decimal_literals.begin(); it != decimal_literals.end(); it++){
                cout << it->first << ": " << it->second << endl;
            }
        }

        if(options.count("-hex")) {
            cout << endl;
            cout << "List of Hex-Literals and their number of occurrences:" << endl;
            for(it = hexadecimal_literals.begin(); it != hexadecimal_literals.end(); it++){
                cout << it->first << ": " << it->second << endl;
            }
        }

        if(options.count("-oct")) {
            cout << endl;
            cout << "List of Octal-Literals and their number of occurrences:" << endl;
            for(it = octal_literals.begin(); it != octal_literals.end(); it++){
                cout << it->first << ": " << it->second << endl;
            }
        }
    }

    file.close();
    return 0;
}
