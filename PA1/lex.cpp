/*==========================================
Christopher Lombardi
PA1
CS 280
10/10/24
============================================*/


#include <iostream>
#include <string>
#include <cstdlib>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <set>
#include <algorithm>

#include "lex.h"


using namespace std;

//stores string version of token
static map<Token, string> tokens = {
    {IF, "IF"},
    {ELSE, "ELSE"},
    {PRINT, "PRINT"},
    {INT, "INT"},
    {FLOAT, "FLOAT"},
    {CHAR, "CHAR"},
    {STRING, "STRING"},
    {BOOL, "BOOL"},
    {PROGRAM, "PROGRAM"},
    {TRUE, "TRUE"},
    {FALSE, "FALSE"},
    
    {IDENT, "IDENT"},
    {ICONST, "ICONST"},
    {RCONST, "RCONST"},
    {SCONST, "SCONST"},
    {BCONST, "BCONST"},
    {CCONST, "CCONST"},

    {PLUS, "PLUS"},
    {MINUS, "MINUS"},
    {MULT, "MULT"},
    {DIV, "DIV"},
    {ASSOP, "ASSOP"},
    {EQ, "EQ"},
    {NEQ, "NEQ"},
    {ADDASSOP, "ADDASSOP"},
    {SUBASSOP, "SUBASSOP"},
    {MULASSOP, "MULASSOP"},
    {DIVASSOP, "DIVASSOP"},
    {REMASSOP, "REMASSOP"},
    {GTHAN, "GTHAN"},
    {LTHAN, "LTHAN"},
    {AND, "AND"},
    {OR, "OR"},
    {NOT, "NOT"},
    {REM, "REM"},

    {COMMA, "COMMA"},
    {SEMICOL, "SEMICOL"},
    {LPAREN, "LPAREN"},
    {RPAREN, "RPAREN"},
    {DOT, "DOT"},
    {LBRACE, "LBRACE"},
    {RBRACE, "RBRACE"},

    {ERR, "ERR"},

    {DONE, "DONE"}

};

//stores keywords
static map<string, Token> kw_map = {
    {"if", IF},
    {"else", ELSE},
    {"print", PRINT},
    {"int", INT},
    {"float", FLOAT},
    {"char", CHAR},
    {"string", STRING},
    {"bool", BOOL},
    {"program", PROGRAM},
    {"true", TRUE},
    {"false", FALSE},
    {"ident", IDENT}

};

static set<Token> keywords = {IF, ELSE, PRINT, INT, FLOAT, CHAR, STRING, BOOL, PROGRAM, TRUE, FALSE,
};

LexItem id_or_kw(const string& lexeme, int linenum){
    Token tok = IDENT;
    string lexlower = lexeme;

    transform(lexlower.begin(), lexlower.end(), lexlower.begin(), ::tolower);

    auto kwFind = kw_map.find(lexlower);

    if(kwFind != kw_map.end()){
        tok = kwFind->second;

        if(tok == TRUE || tok == FALSE){
            return LexItem(BCONST, lexeme, linenum);
        }
        else{
            return LexItem(tok, lexeme, linenum);
        }
    }
    return LexItem(IDENT, lexeme, linenum);
}

//overloaded << operator
ostream& operator<<(ostream& out, const LexItem& tok){
    Token token = tok.GetToken();
    if(token == ICONST || token == RCONST || token == BCONST){
        return out <<  tokens[token] << ": (" << tok.GetLexeme() << ")";
    }
    else if(token == IDENT){
        return out << tokens[token] << ": <" << tok.GetLexeme() << ">";
    }
    else if(token == SCONST){
        return out << tokens[token] << ": \"" << tok.GetLexeme() << "\"";
    }
    else if (token == CCONST){
        return out << tokens[token] << ": \'" << tok.GetLexeme().substr(1, tok.GetLexeme().size()-2) << "\'";
    }
    else if (token == ERR){
        return out << tokens[token] << ": In line " << tok.GetLinenum() << ", Error Message {" << tok.GetLexeme() <<"}"; 
    }
    else{
        return out << tokens[token];
    }
    return out;

}

LexItem getNextToken(istream& in, int& lineNumber) {
    enum LexState {START, INID, INSIGN, ININT, INMULTCOMM, INCOMM, INNUM, INREAL, INSTRING} lexstate = START;
    string lexeme;
    char ch;
    //bool hasSign = false; //to see if signed int

    while (in.get(ch)) {
        switch (lexstate) {
            case START:
                //lexeme.clear();
                if (isspace(ch)) {
                    if (ch == '\n') lineNumber++;
                    continue;
                }
                if (isalpha(ch) || ch == '_') {
                    lexeme = ch;
                    lexstate = INID;
                } else if (isdigit(ch)) {
                    lexeme = ch;
                    lexstate = INNUM;
                } 
                else if (ch == '"') {
                    lexeme = ch;
                    lexstate = INSTRING;  
                } else if (ch == '\'') {
                    lexeme = ch;
                    in.get(ch);
                    lexeme += ch;
                    in.get(ch);  
                    if (ch == '\'') {
                        if(in.peek() == '\n'){
                            return LexItem(ERR, "New Line is an invalid character constant.", lineNumber);
                        }
                        lexeme += ch;
                        return LexItem(CCONST, lexeme, lineNumber);}
                        else{
                            lexeme += ch;
                            return LexItem(ERR, " Invalid character constant " +lexeme + '\'', lineNumber);
                        }
                    
                    return LexItem(ERR, lexeme, lineNumber);
                } else {
                    switch (ch) {
                        //string cases
                        case '!':
                            if(in.peek() == '='){
                                lexeme = ch;
                                lexeme += in.get();
                                return LexItem(NEQ, lexeme, lineNumber);
                            }
                            else{
                                return LexItem(NOT, lexeme, lineNumber);
                            }
                        case '&':
                            if(in.peek() == '&'){
                                lexeme = ch;
                                lexeme += in.get();
                                return LexItem(AND, lexeme, lineNumber);
                            }
                            else{
                                return LexItem(ERR, string(1, ch), lineNumber);
                            }
                        case '|':
                            if(in.peek() == '|'){
                                lexeme = ch;
                                lexeme += in.get();
                                return LexItem(OR, lexeme, lineNumber);
                            }
                            else{
                                return LexItem(ERR, string(1, ch), lineNumber);
                            }
                        case '/':
                            lexeme = ch;
                            if(in.peek() == '/'){
                                lexstate = INCOMM;
                                in.get();
                            }
                            else if(in.peek() == '*'){
                                in.get();
                                lexstate = INMULTCOMM;
                            }
                            else if(in.peek() == '='){
                                lexeme += in.get();
                                return LexItem(DIVASSOP, lexeme, lineNumber);
                            }
                            else{
                                return LexItem(DIV, lexeme, lineNumber);
                            }
                            break;

                        case '+': 
                            lexeme = ch;
                            if(in.peek() == '='){
                                
                                lexeme += in.get();
                                return LexItem(ADDASSOP, lexeme, lineNumber);
                                }
                            else if(isdigit(in.peek())){
                                lexstate = INSIGN;}
                            else{
                              return LexItem(PLUS, lexeme, lineNumber);  
                            } 
                       case '-': 
                            lexeme = ch;  
                            if (in.peek() == '=') {  
                            lexeme += in.get();
                            return LexItem(SUBASSOP, lexeme, lineNumber);
                        } else {
                            in.get(ch); 
                            while (isspace(ch)) {  
                                if (ch == '\n') lineNumber++;  
                                in.get(ch);
                                    }
                            if (isdigit(ch)) {  
                                lexeme += ch;
                                lexstate = INNUM;
                                } 
                            else if (ch == '.') { 
                            lexeme += ch;
                            lexstate = INREAL;  
                            } 
                            else {  
                            in.putback(ch);
                            return LexItem(MINUS, lexeme, lineNumber);
                            }
                            }
                            break;
                        case '*': 
                            if(in.peek() == '='){
                                in.get();
                                lexeme = ch;
                                lexeme += in.get();
                                return LexItem(MULASSOP, lexeme, lineNumber);
                            }
                            else{
                                return LexItem(MULT, lexeme, lineNumber);
                            }
                        case '=': 
                            if(in.peek() == '='){
                                lexeme = ch;
                                lexeme += in.get();
                                return LexItem(EQ, lexeme, lineNumber);}
                            else{ 
                                return LexItem(ASSOP, lexeme, lineNumber);
                            }
                        case '%':
                            if(in.peek() == '='){
                                lexeme = ch;
                                lexeme += in.get();
                                return LexItem(REMASSOP, lexeme, lineNumber);
                            }
                            else{
                                return LexItem(REM, lexeme, lineNumber);
                            }
                        case ',': return LexItem(COMMA, lexeme, lineNumber);
                        case ';': return LexItem(SEMICOL, lexeme, lineNumber);
                        case '.':
                            if (isdigit(in.peek())) {  
                                lexeme = ch;  
                            while (isdigit(in.peek())) { 
                                lexeme += in.get();
                             }
                                return LexItem(RCONST, lexeme, lineNumber);  
                            } else {
                                return LexItem(DOT, ".", lineNumber);  
                            }
                                break;

                        case '<': return LexItem(LTHAN, lexeme, lineNumber);
                        case '>': return LexItem(GTHAN, lexeme, lineNumber);
                        case '(': return LexItem(LPAREN, lexeme, lineNumber);
                        case ')': return LexItem(RPAREN, lexeme, lineNumber);
                        case '}': return LexItem(RBRACE, lexeme, lineNumber);
                        case '{': return LexItem(LBRACE, lexeme, lineNumber);
                        default:
                            return LexItem(ERR, string(1, ch), lineNumber);
                    }
                }
                break;

            case INSIGN:
                lexeme = ch;
                in.get(ch);
                while(isspace(ch)){
                    in.get(ch);
                }
                if (isdigit(ch)) {
                    lexeme += ch;
                    lexstate = INNUM;
                } else if (ch == '.') {
                    lexeme += ch;
                    lexstate = INREAL; 
                } else {
                    in.putback(ch);
                    return LexItem(ERR, lexeme, lineNumber);
                }
                break;

            case INID:
                if (isalnum(ch) || ch =='_') {
                    lexeme += ch;
                } else {
                    in.putback(ch);
                    return id_or_kw(lexeme, lineNumber);
                }
                break;

            case INNUM:
                if (isdigit(ch)) {
                    lexeme += ch;
                } else if (ch == '.') {
                    if(isspace(in.peek())){
                        in.putback(ch);
                        return LexItem(ICONST, lexeme, lineNumber);
                    }
                    else{
                        lexeme +=ch;
                        lexstate = INREAL;
                    }
                } else {
                    in.putback(ch);
                    return LexItem(ICONST, lexeme, lineNumber);  
                }
                break;

            case INREAL:
                if (isdigit(ch)) {
                    lexeme += ch;
                }
                else if(ch =='.'){
                    lexeme += ch;
                    return LexItem(ERR, lexeme, lineNumber);
                }
                
                 else {
                    in.putback(ch);
                    return LexItem(RCONST, lexeme, lineNumber);
                }
                break;

           case INSTRING:
                if (ch == '\n') {
                    return LexItem(ERR, " Invalid string constant " + lexeme, lineNumber);
                }
                else if (ch!= '"') {
                    lexeme += ch;
                }
                else {
                    return LexItem(SCONST, lexeme.substr(1, lexeme.length()), lineNumber);  
               
                }
                break;

            case INMULTCOMM:
                if (ch == '\n') {
                    lineNumber++;
                    }
                if (ch == '*' && in.peek() == '/') {
                    in.get();  
                    lexstate = START;
                }
                break;

            case INCOMM:
                if (ch == '\n') {
                    lineNumber++;
                    lexstate = START;
                }
                break;

            default:
                break;
        }
    }

    return LexItem(DONE, "", lineNumber);
}
