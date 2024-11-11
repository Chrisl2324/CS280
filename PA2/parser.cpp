#include "parser.h"

map<string, bool> defVar;
map<string, Token> SymTable;

int nesting_level = 0;

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

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

bool IdentList(istream& in, int& line);

//Prog ::= PROGRAM IDENT CompStmt
bool Prog(istream& in, int& line){
	LexItem t;
	bool status;

	t = Parser::GetNextToken(in, line);
	if(t != PROGRAM){
		ParseError(line, "Missing Program keyword");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t != IDENT){
		ParseError(line, "Missing program name");
		return false;
	}
	//cout << "line: " << line << " before calling CompStmt" << endl;
	status = CompStmt(in, line);
	if(!status){
		ParseError(line, "Invalid Program");
		return false;
	}
	cout << "(DONE)" <<endl;
	return true;
}

// StmtList ::= Stmt { Stmt }
bool StmtList(istream& in, int& line) {
    bool status = false;
    LexItem tok;

    //cout << "line: " << line << " Before calling Stmt" << endl;

    status = Stmt(in, line); // Attempt to parse a statement
    if (!status) {
        ParseError(line, "Missing Stmt");
        return false;
    }
    //cout << "Line: " << line << " After calling Stmt" << endl;

    // Loop to parse additional statements
    tok = Parser::GetNextToken(in, line);
    while (tok != RBRACE && tok != DONE) { 
        Parser::PushBackToken(tok); 
        status = Stmt(in, line); // Parse the next statement
        if (!status) {
            ParseError(line, "Syntactic Error in statement list.");
            return false;
        }
        tok = Parser::GetNextToken(in, line); // Get the next token
    }

    if (tok == RBRACE) {
        Parser::PushBackToken(tok); // Push back the closing brace for CompStmt to handle
    } else if (tok != DONE) {
        ParseError(line, "Missing Right Braces");
        return false;
    }

    return true;
}

// Stmt ::= DeclStmt | ControlStmt | CompStmt
bool Stmt(istream& in, int& line) {
    LexItem t = Parser::GetNextToken(in, line);
    //cout << "line: " << line << " calling Stmt: " << t.GetLexeme() << endl; // Debug statement
    if(t.GetLexeme() == ","){
        ParseError(line, "Missing Semicolon");}
    switch (t.GetToken()) {
        case INT: case FLOAT: case BOOL: case CHAR: case STRING:
            //cout << "line: " << line << " calling DeclStmt" << endl;
            Parser::PushBackToken(t);
            if (!DeclStmt(in, line)) {
                ParseError(line, "Incorrect Declaration Statement");
                return false;
            }
			break;
        case IF: case PRINT: case IDENT:
            //cout << "line: " << line << " calling ControlStmt" << endl;
            Parser::PushBackToken(t);
            if (!ControlStmt(in, line)) {
                ParseError(line, "Invalid Control Statement");
                return false;
            }
            break;
        case LBRACE:
            //cout << "line: " << line << " calling CompStmt" << endl;
            Parser::PushBackToken(t);
            if (!CompStmt(in, line)) {
                ParseError(line, "Invalid Compound Statement");
                return false;
            }
            break;
        default:
            Parser::PushBackToken(t);
            //ParseError(line, "Invalid statement");
            return false;
    }
    return true;
}



//DeclStmt ::= ( INT | FLOAT | BOOL | CHAR | STRING ) VarList ; 
bool DeclStmt(istream& in, int& line){
	//cout << "line: " << line << " in DeclStmt" << endl;
	bool status = false;
	LexItem t;
	t = Parser::GetNextToken(in, line);
	//cout << t.GetLexeme() << " from DeclStmt" << endl;

	if(t!= INT && t!= FLOAT && t!= BOOL && t!= CHAR && t!= STRING){
		ParseError(line, "Invalid Type");
		return false;
	}
	//cout << "line: " << line << " before calling VarList" << endl;
	
	status = VarList(in, line);
	//cout << "line: " << line << " after calling VarList" << endl;
	if(!status){
		ParseError(line, "Incorrect Variable in VarList");
		return false;
	}
	
	//cout << t.GetLexeme() <<  " from DeclStmt2" << endl;
	
	return true;
}

// VarList ::= Var [= Expr] { , Var [= Expr] }
bool VarList(istream& in, int& line) {
	//cout << "in varList" << endl;
    LexItem t;
    bool status = false;

    // Parse the first variable
    t = Parser::GetNextToken(in, line);
    if (t != IDENT) {
        ParseError(line, "Must be an Identifier");
        return false;
    }

    // Check for redefinition
    if (defVar.find(t.GetLexeme()) != defVar.end()) {
        ParseError(line, "Variable Redefinition.");
        return false;
    } else {
        defVar[t.GetLexeme()] = true;  // Declare the variable
    }
	string name = t.GetLexeme();
    // Handle optional initialization
    t = Parser::GetNextToken(in, line);
    if (t == ASSOP) {
		//cout << "calling Expr" << endl;
        status = Expr(in, line);
        if (!status) {
            ParseError(line, "Missing Expr after '='");
            return false;
        }
		cout << "Initialization of the variable " << name << " in the declaration statement at line " << line << endl;

        t = Parser::GetNextToken(in, line);
    }

    // Handle multiple variables separated by commas
    while (t == COMMA) {
        t = Parser::GetNextToken(in, line);
        if (t != IDENT) {
            ParseError(line, "Expected identifier after ','");
            return false;
        }

        // Check for redefinition
        if (defVar.find(t.GetLexeme()) != defVar.end()) {
            ParseError(line, "Variable '" + t.GetLexeme() + "' already declared");
            return false;
        } else {
            defVar[t.GetLexeme()] = true;  // Declare the next variable
        }
		string name = t.GetLexeme();
        // Handle optional initialization
        t = Parser::GetNextToken(in, line);
        if (t == ASSOP) {
            status = Expr(in, line);
            if (!status) {
                ParseError(line, "Missing Expr after '='");
                return false;
            }
			cout << "Initialization of the variable " << name << " in the declaration statement at line " << line << endl;

            t = Parser::GetNextToken(in, line);
        }
    }

    

    return true;
}


//CompStmt ::= '{'StmtList'}'
bool CompStmt(istream& in, int& line){
	LexItem t;
	bool status;

	t = Parser::GetNextToken(in, line);
	if(t != LBRACE){
		
		return false;
	}
	//cout << "line: " << line << " calling StmtList " << endl;
	status = StmtList(in, line);
	if(!status){
		ParseError(line, "Incorrect Statement list.");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t!= RBRACE){
		ParseError(line, "Missing Right Braces");
		return false;
	}
	return true;
}

//PrintStmt:= PRINT (ExprList) 
bool PrintStmt(istream& in, int& line) {
	LexItem t;
	//cout << "in PrintStmt" << endl;
	t = Parser::GetNextToken(in, line);
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		//cout << "IN print statement" << endl;
		//cout << t.GetLexeme() << endl;
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Incorrect Print Statement");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	

	return true;
}//End of PrintStmt


//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	//cout << "in ExprList and before calling Expr" << endl;
	status = Expr(in, line);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		//cout << "before calling ExprList" << endl;
		status = ExprList(in, line);
		//cout << "after calling ExprList" << endl;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		//cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}//End of ExprList

//Var := IDENT
bool Var(istream& in, int& line){
	//cout << "line: " << line << " in Var" << endl;
	LexItem t;
	t = Parser::GetNextToken(in, line);
	//cout << t.GetLexeme() << " in VAr" << endl;
	if(t != IDENT){
		ParseError(line, "Must be an Identifier");
		return false;
	}
	if(defVar.find(t.GetLexeme()) == defVar.end()){
		ParseError(line, "Undefined Variable");
		return false;
		}

	return true;
}
//AssgnStmt ::= Var ( = | += | -= | *= | /= | %= ) Expr
bool AssignStmt(istream& in, int& line) {
    LexItem t;
    bool status = false;
    //cout << "line: " << line << " In AssignStmt" << endl;

    // Parse left-hand variable
    status = Var(in, line);
    if (!status) {
        ParseError(line, "Missing left-hand side variable in assignment statement");
        return false;
    }

    // Get assignment operator
    t = Parser::GetNextToken(in, line);
    if (t != ASSOP && t != MULASSOP && t != DIVASSOP && t != ADDASSOP && t != REMASSOP && t != SUBASSOP) {
        ParseError(line, "Missing assignment operator");
        return false;
    }

    // Parse the right-hand side expression
    status = Expr(in, line);
    if (!status) {
        ParseError(line, "Missing expression in assignment statement");
        return false;
    }

    return true;
}

//ControlStmt ::= AssgnStmt ; | IfStmt | PrintStmt ;
bool ControlStmt(istream& in, int& line) {
    LexItem t;
    bool status = false;

	t = Parser::GetNextToken(in, line); 

	switch(t.GetToken()){
		case IDENT:
			Parser::PushBackToken(t);
			//cout << "calling AssignStmt" << endl;
			status = AssignStmt(in, line);
			if(!status){
				ParseError(line, "Incorrect Assignment Statement");
				return false;
			}
			break;
		case IF:
			Parser::PushBackToken(t);
			status = IfStmt(in, line);
			if(!status){
				ParseError(line, "Incorrect IF statement");
				return false;
			}
			return true;
			break;
		case PRINT:
			Parser::PushBackToken(t);
			status = PrintStmt(in, line); 
			if(!status){
				ParseError(line, "Incorrect Print statement");
				return false;
			}
			break;
		default:
			ParseError(line, "Invalid control statement.");
    		return false;
			break;
	}
	t = Parser::GetNextToken(in, line);
	if(t!= SEMICOL){
        line --;
		ParseError(line, "Missing semicolon after statement.");
		return false;
	}
	return true;
}


bool IfStmt(istream& in, int& line) {
    //cout << "In IfStmt" << endl;
    LexItem t;
    bool status = false;

    // Check if the first token is IF
    t = Parser::GetNextToken(in, line);
    if (t != IF) {
        ParseError(line, "Missing IF keyword");
        return false;
    }

    t = Parser::GetNextToken(in, line);
    if (t != LPAREN) {
        ParseError(line, "Missing Left Parentheses after IF");
        return false;
    }

    status = Expr(in, line);
    if (!status) {
        ParseError(line, "Missing Expression in If Statement");
        return false;
    }

    t = Parser::GetNextToken(in, line);
    if (t != RPAREN) {
        ParseError(line, "Missing Right Parentheses");
        return false;
    }

    nesting_level++;
    cout << "in IfStmt then-clause at nesting level: " << nesting_level << endl;

    status = Stmt(in, line);
    if (!status) {
        ParseError(line, "Missing statement after expression in IF");
        return false;
    }

    // Check for an optional ELSE
    t = Parser::GetNextToken(in, line);
    if (t == ELSE) {
        cout << "in IfStmt else-clause at nesting level: " << nesting_level << endl;

        status = Stmt(in, line);
        if (!status) {
            ParseError(line, "Missing statement after ELSE");
            return false;
        }
    } else {
        // Push back the token if it's not ELSE, for further parsing
        Parser::PushBackToken(t);
    }

    // Decrement nesting level as we're leaving the if block
    nesting_level--;

    return true;
}


bool PrimaryExpr(istream& in, int& line, int sign) {
    LexItem tok = Parser::GetNextToken(in, line);
    //cout << "line: " << line << " in PrimaryExpr: " << tok.GetLexeme() << endl;

    if (tok == IDENT || tok == ICONST || tok == RCONST || tok == SCONST || tok == BCONST || tok == CCONST) {
        return true;  
    }
	
    // Handle (Expr)
    if (tok == LPAREN) {
        bool status = Expr(in, line);
        if (!status) {
            ParseError(line, "Missing expression inside parentheses in PrimaryExpr");
            return false;
        }
        tok = Parser::GetNextToken(in, line);
        if (tok != RPAREN) {
            ParseError(line, "Missing closing ')' in PrimaryExpr");
            return false;
        }
        return true;  // Successfully parsed (Expr).
    }

    //ParseError(line, "Invalid PrimaryExpr");
    return false;
}


//UnaryExpr ::= [(-|+|!)] PrimaryExpr
bool UnaryExpr(istream& in, int& line){
	LexItem t;
	bool status = false;

	t = Parser::GetNextToken(in, line);
	//cout << "line: " << line << " calling primaryExpr" << endl;
	
	if(t == MINUS){
		status = PrimaryExpr(in, line, MINUS);
		if(!status){
			ParseError(line, "Incorrect Primary Expression");
			return false;
		}
	}
	else if(t == PLUS){
		status = PrimaryExpr(in, line, PLUS);
		if(! status){
			ParseError(line, "Incorrect Primary Expression");
			return false;
		}
	}
	else if(t == NOT){
		status = PrimaryExpr(in, line, NOT);
		if(! status){
			ParseError(line, "Incorrect Primary Expression");
			return false;
		}
	}
	else{
		Parser::PushBackToken(t);
		status = PrimaryExpr(in, line, PLUS);
		if(! status){
			//ParseError(line, "Incorrect Primary Expression");
			return false;
		}
	}
	return true;

}


// LogANDExpr ::= EqualExpr { && EqualExpr }
bool LogANDExpr(istream& in, int& line) {
    //cout << "line: " << line << " calling equal expression" <<endl;
    bool status = EqualExpr(in, line);
    if (!status) {
        //ParseError(line, "Missing EqualExpr");
        return false;
    }

    LexItem t = Parser::GetNextToken(in, line);

    while (t == AND) {
        // Parse the next EqualExpr
        status = EqualExpr(in, line);
        if (!status) {
            ParseError(line, "Missing EqualExpr after '&&'");
            return false;
        }
        // Get the next token to check for another '&&'
        t = Parser::GetNextToken(in, line);
    }

    Parser::PushBackToken(t);
    return true;
}

//EqualExpr ::= RelExpr [(== | !=) RelExpr]
bool EqualExpr(istream& in, int& line) {
    LexItem t;
    bool status;
    //cout << "line: " << line << " Calling RelEXpr" << endl;

    // First RelExpr
    status = RelExpr(in, line);
    if (!status) {
        //ParseError(line, "Missing RelExpr");
        return false;
    }

    t = Parser::GetNextToken(in, line);

    if (t == EQ || t == NEQ) {
        //cout << "Found equality operator: " << t.GetLexeme() << endl;

        status = RelExpr(in, line);
        if (!status) {
            ParseError(line, "Missing RelExpr after equality operator");
            return false;
        }

        LexItem next = Parser::GetNextToken(in, line);
        if (next == EQ || next == NEQ) {
            ParseError(line, "Illegal Equality Expression.");
            return false;
        } else {
            Parser::PushBackToken(next); // Valid token, push it back
        }
    } else {
        Parser::PushBackToken(t); // No equality operator, push back token
    }

    return true;
}

// AddExpr ::= MultExpr { ( + | - ) MultExpr }
bool AddExpr(istream& in, int& line) {
    bool status = false;

    //cout << "line: " << line << " calling MultExpr" << endl;
    status = MultExpr(in, line);
    if (!status) {
        //ParseError(line, "Missing MultExpr in AddExpr");
        return false;
    }

    LexItem t = Parser::GetNextToken(in, line);
    while (t == PLUS || t == MINUS) {
        status = MultExpr(in, line);
        if (!status) {
            ParseError(line, "Missing MultExpr after '+' or '-' in AddExpr");
            return false;
        }
        t = Parser::GetNextToken(in, line);
    }

    Parser::PushBackToken(t);

    return true;
}



//RelExpr ::= AddExpr [ ( < | > ) AddExpr ]
bool RelExpr(istream& in, int& line){
    bool status = false;
    //cout << "line: " << line << " Calling AddExpr" << endl;

    // First AddExpr
    status = AddExpr(in, line);
    if (!status) {
        //ParseError(line, "Missing AddExpr");
        return false;
    }

    LexItem t = Parser::GetNextToken(in, line);

    if (t == LTHAN || t == GTHAN) {
        //cout << "line: " << line << " Found relational operator: " << t.GetLexeme() << endl;

        // Parse the next AddExpr
        status = AddExpr(in, line);
        if (!status) {
            ParseError(line, "Missing AddExpr after relational operator");
            return false;
        }

        LexItem next = Parser::GetNextToken(in, line);
        if (next == LTHAN || next == GTHAN) {
            ParseError(line, "Illegal Relational Expression.");
            return false;  // Error: Cannot chain relational operators
        } else {
            Parser::PushBackToken(next);  // Push back valid token for further parsing
        }
    } else {
        Parser::PushBackToken(t);  // No relational operator, push back token
    }

    return true;
}

//Expr ::= LogANDExpr { || LogAndExpr }
bool Expr(istream& in, int& line){
	bool status = false;
	//cout << "line: " << line << "calling logANDExpr" << endl;
	status = LogANDExpr(in, line);

	if(!status){
		//ParseError(line, "Missing LogANDExpr");
		return false;
	}
	LexItem t;
	t = Parser::GetNextToken(in, line);
	//cout << t.GetLexeme() << " from Expr" << endl;
	while(t == OR){
		//cout << "line: " << line << " calling LogANDExpr" << endl;
		status = LogANDExpr(in, line);
		if(!status){
			ParseError(line, "Incorrect Expr");
			return false;
		}
		t = Parser::GetNextToken(in, line);
	}
	Parser::PushBackToken(t);
	return true;
}

//MultExpr ::= UnaryExpr {(* | / | %) UnaryExpr}
bool MultExpr(istream& in, int& line) {
    bool status = false;

    //cout << "line: " << line << " calling UnaryExpr" << endl;
    status = UnaryExpr(in, line);  // First UnaryExpr
    if (!status) {
        //ParseError(line, "Missing UnaryExpr in MultExpr");
        return false;
    }

    LexItem t = Parser::GetNextToken(in, line);  // Get the next token after UnaryExpr

    while (t == MULT || t == DIV || t == REM) {
        //cout << "line: " << line << " Found operator: " << t.GetLexeme() << endl;

        status = UnaryExpr(in, line);
        if (!status) {
            ParseError(line, "Missing operand after operator '" + t.GetLexeme() + "'");
            return false;
        }

        t = Parser::GetNextToken(in, line);
    }

    Parser::PushBackToken(t);

    return true;
}







