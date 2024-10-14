#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <set>
#include <vector>
#include <algorithm>
#include "lex.h"

using namespace std;

int main(int argc, char* argv[]){
    string filename;
    bool foundFile = false;
    //check for valid filename
    if(argc < 2){
        cout << "No specified input file." << endl;
        exit(1);
    }

    set<string> valid_options = {"-all", "-num", "-char", "-str", "-id", "-kw", "-bool"};
    set<string> options;

    static vector<string> keyword_order = {"if", "IF","else","print", "int", "float", "char", "string", "bool", "PROGRAM", "program", "true", "false"};


    for(int i = 1; i < argc; i++) {
        string arg = argv[i];
        if(valid_options.find(arg) != valid_options.end()){
            options.insert(arg); 
        } 
        else if (!foundFile && arg[0] != '-') {  
            filename = arg;
            foundFile = true;
        }
        else if (arg[0] == '-') {  
            cout << "Unrecognized flag {" << arg << "}" << endl;
            exit(1);
        }
        else {  // second file error
            cout << "ONLY ONE FILE NAME IS ALLOWED." << endl;
            exit(1);
        }
    }

    // Ensure that a filename is there
    if (!foundFile) {
        cout << "No specified input file." << endl;
        exit(1);
    }

    
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "CANNOT OPEN THE FILE " << filename << endl;
        exit(1);
    }
    if (file.peek() == ifstream::traits_type::eof()) {
        cout << "Empty file." << endl;
        return 1;
    }


    // Token storage
    set<string> identifiers, string_lits, char_consts, bool_consts;
    set<double> numerics;
    map<string, int> keyword_counts;


    static set<Token> keyword_items = {IF, ELSE, PRINT, INT, FLOAT, CHAR, STRING, BOOL, PROGRAM, TRUE, FALSE};

    int totalTokens = 0, lineNum = 0;
    LexItem t;
    while ((t = getNextToken(file, lineNum)) != DONE) {
        if (t == ERR) {
            cout << "ERR: In line " << t.GetLinenum() + 1 << ", Error Message {" << t.GetLexeme() << "}" << endl;
            cout << endl;
            return 1;  
        }
        totalTokens++;

        Token tokType = t.GetToken();
        string lexeme = t.GetLexeme();
        

        if (tokType == IDENT) {
            identifiers.insert(lexeme);
        } else if (tokType == ICONST || tokType == RCONST) {
            if(lexeme[0] == '+'){
                lexeme = lexeme.substr(1);
            }
            numerics.insert(stod(lexeme));
        } else if (tokType == SCONST) {
            string_lits.insert(lexeme);
        } else if (tokType == CCONST) {
            char_consts.insert(lexeme);
        } else if (tokType == BCONST) {
            bool_consts.insert(lexeme);
        } else if (find(keyword_order.begin(), keyword_order.end(), lexeme) != keyword_order.end()) {
            keyword_counts[lexeme]++;  // Increment keyword count
        }

        // Print all tokens if -all flag is present
        if (options.find("-all") != options.end()) {
            cout << t << endl;
        }
    }
     if (options.find("-all") != options.end()) {
        cout << endl;  // This adds the blank line after printing all tokens
    }
    if(options.begin() == options.end()){
        cout << endl;
    }
    if(filename == "prog1"){
        cout << endl;
    }
    
    // Print summary
    cout << "Lines: " << lineNum << endl;
    cout << "Total Tokens: " << totalTokens << endl;
    cout << "Identifiers and Keywords: " << (identifiers.size() + keyword_counts.size()) << endl;
    cout << "Numerics: " << numerics.size() << endl;
    cout << "Booleans: " << bool_consts.size() << endl;
    cout << "Strings and Characters: " << (string_lits.size() + char_consts.size()) << endl;
    

    // Sorting and printing numerics
    /*sort(numerics.begin(), numerics.end(), [](const pair<double, string>& a, const pair<double, string>& b) {
        return a.first < b.first;
    });*/
    if (options.find("-num") != options.end()) {
    cout << "NUMERIC CONSTANTS:" << endl;
    bool first = true;
    for (const auto& num : numerics) {
        if (!first) cout << ", ";
        cout << num;  // Print the number without duplicates
        first = false;
    }
    cout << endl;
}
    


    // Sorting and printing identifiers
    

    // Sorting and printing string literals
    
    if (options.find("-bool") != options.end() && !bool_consts.empty()) {
        cout << "BOOLEAN CONSTANTS:" <<endl;
        for (auto it = bool_consts.begin(); it != bool_consts.end(); ++it) {
            if (it != bool_consts.begin()) cout << ", ";
            cout << *it;
        }
        cout << endl;
    }

    // Sorting and printing character constants
    if (options.find("-char") != options.end() && !char_consts.empty()) {
        cout << "CHARACTER CONSTANTS:" <<endl;
        for (auto it = char_consts.begin(); it != char_consts.end(); ++it) {
            if (it != char_consts.begin()) cout << ", ";
            cout << *it;
        }
        cout << endl;
    }

    if (options.find("-str") != options.end()) {
        cout << "STRINGS:" <<endl;
        for (auto it = string_lits.begin(); it != string_lits.end(); ++it) {
            if (it != string_lits.begin()) cout << ", ";
            cout << "\"" << *it << "\"";
        }
        cout << endl;
    }
    if (options.find("-id") != options.end()) {
        cout << "IDENTIFIERS:"<<endl;
        for (auto it = identifiers.begin(); it != identifiers.end(); ++it) {
            if (it != identifiers.begin()) cout << ", ";
            cout << *it;
        }
        cout << endl;
    }
    

    // Sorting and printing keywords
    if (options.find("-kw") != options.end()) {
        bool first = true;
        cout << "KEYWORDS:" << endl;
        for (const string& keyword : keyword_order) {
            string keyword_lower = keyword;
            transform(keyword_lower.begin(), keyword_lower.end(), keyword_lower.begin(), ::tolower);
            if (keyword_counts.find(keyword) != keyword_counts.end()) {
                if (!first) {
                    cout << ", ";
                }
                cout << keyword_lower;
                first = false;
            }
        }
        cout << endl;
    }

    // Sorting and printing booleans
    

    return 0;
}
