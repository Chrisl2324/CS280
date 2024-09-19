/*===========================================
Christopher Lombardi
CS280
RA3
09/16/24
============================================*/


#include <iostream>
#include <string>
#include <regex>
#include <cstdlib>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[]) {
    string filename;

    if (argc < 2) {
        cout << "NO SPECIFIED INPUT FILE NAME." << endl;
        return 1;
    } else {
        filename = argv[1];  // Get filename from command line argument
    }

    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Cannot open the file: " << filename << "." << endl;
        return 1;
    }

    // Process file as normal

    

    string line;

    // Valid regex patterns
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

    while (getline(file, line)) {
        numLines++;

        istringstream iss(line);
        string word;

        while (iss >> word) {
            numWords++;

            if (regex_match(word, invalid_binary)) {
                cout << "Incorrect Binary Literal " << word << " at Line: " << numLines << endl;
            } else if (regex_match(word, invalid_hexa)) {
                cout << "Incorrect Hexadecimal Literal " << word << " at Line: " << numLines << endl;
            } else if (regex_match(word, invalid_octa)) {
                cout << "Incorrect Octal Literal " << word << " at Line: " << numLines << endl;
            }
            else if(regex_match(word, invalid_decimal)){
                cout << "Incorrect Decimal Literal " << word << " at Line: " << numLines << endl;
            }
            else if (regex_match(word, decimal)) {
                numDecimals++;
            } else if (regex_match(word, hexa)) {
                numHexa++;
            } else if (regex_match(word, octa)) {
                numOctal++;
            } else if (regex_match(word, binary)) {
                numBinary++;
            }
        }
    }

    if (numLines == 0) {
        cout << "File is empty." << endl;
        exit(1);
    }

    // Output results
    cout << "Total Number of Lines: " << numLines << endl;
    cout << "Number of Words: " << numWords << endl;
    cout << "Number of Decimal Literals: " << numDecimals << endl;
    cout << "Number of Hexadecimal Literals: " << numHexa << endl;
    cout << "Number of Octal Literals: " << numOctal << endl;
    cout << "Number of Binary Literals: " << numBinary << endl;

    file.close();
    return 0;
}
