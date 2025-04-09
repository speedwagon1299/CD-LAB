%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

%}

%token FUNCTION INT CHAR BOOL TRUE FALSE IF ELSE NUM LIT ID NL 

%%

Program : FunctionDecl NL {
    printf("\nValid\n");
    exit(0);
}
;
FunctionDecl : FUNCTION ID '(' ')' '{' StmtList '}'
;
StmtList : Stmt StmtList | 
;
Stmt : Declaration | Assignment | IfStmt
;
Declaration : Type ID InitOpt ';'
;
Assignment : ID '=' Expr ';'
;
IfStmt : IF '(' Expr ')' '{' StmtList '}' ElsePart
;
ElsePart : ELSE '{' StmtList '}' | 
;
Type : INT | CHAR | BOOL
;
InitOpt : '=' Expr | 
;
Expr : NUM | LIT | TRUE | FALSE | ID
;


%%

int yyerror(char* msg) {
    printf("\nInvalid: %s\n", msg);
}

void yywrap() {

}

int main() {
    yyparse();
}