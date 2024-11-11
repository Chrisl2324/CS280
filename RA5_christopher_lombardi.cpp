#include <iostream>
#include <string>
#include <cstdlib>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <set>
#include "lex.h"


using namespace std;

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


LexItem id_or_kw(const string& lexeme, int linenum){
    Token tok = IDENT;

    auto kwFind = kw_map.find(lexeme);

    if(kwFind != kw_map.end()){
        tok = kwFind->second;

        if(tok == TRUE || tok == FALSE){
            return LexItem(BCONST, lexeme, linenum);
        } else {
            return LexItem(tok, lexeme, linenum);
        }
    }
    return LexItem(IDENT, lexeme, linenum);
}

static set<Token> keywords = {IF, ELSE, PRINT, INT, FLOAT, CHAR, STRING, BOOL, PROGRAM, TRUE, FALSE,
};


ostream& operator<<(ostream& out, const LexItem& tok){
    Token token = tok.GetToken();

    if(keywords.find(token) != keywords.end()){
        out << tokens[token] << ": KEYWORD";
    }

    if(token == ICONST || token == RCONST || token == BCONST){
        out << tokens[token] << ": (" << tok.GetLexeme() << ")";
    }
    else if(token == IDENT){
        out << tokens[token] << ": <" << tok.GetLexeme() << ">";
    }
    else if(token == SCONST){
        out << tokens[token] << ": \"" << tok.GetLexeme() << "\"";
    }
    else if (token == CCONST){
        out << tokens[token] << ": \'" << tok.GetLexeme() << "\'";
    }
    else if (token == ERR){
        out << tokens[token] << ": {" << tok.GetLexeme() << "} error at line no. " << tok.GetLinenum(); 
    }
    else{
        if(keywords.find(token) == keywords.end()){
            out << tokens[token] << ": [" <<  tok.GetLexeme() << "]";
        }
    }
    return out;

}