/* 
 * parser_Fall24.h
 * Building a Parser for Mini C-Like Language
 * Programming Assignment 2
 * Fall 2024
*/

#ifndef PARSE_H_
#define PARSE_H_

#include <iostream>

using namespace std;

#include "lex.h"



extern bool Prog(istream& in, int& line);
extern bool StmtList(istream& in, int& line);
extern bool Stmt(istream& in, int& line);
extern bool DeclStmt(istream& in, int& line);
extern bool VarList(istream& in, int& line);
extern bool ControlStmt(istream& in, int& line);
extern bool PrintStmt(istream& in, int& line);
extern bool CompStmt(istream& in, int& line);
extern bool IfStmt(istream& in, int& line);
extern bool AssignStmt(istream& in, int& line);
extern bool Var(istream& in, int& line);
extern bool ExprList(istream& in, int& line);
extern bool Expr(istream& in, int& line);
extern bool LogANDExpr(istream& in, int& line);
extern bool EqualExpr(istream& in, int& line);
extern bool RelExpr(istream& in, int& line);
extern bool AddExpr(istream& in, int& line);
extern bool MultExpr(istream& in, int& line);
extern bool UnaryExpr(istream& in, int& line);
extern bool PrimaryExpr(istream& in, int& line, int sign);

extern int ErrCount();

#endif /* PARSE_H_ */
