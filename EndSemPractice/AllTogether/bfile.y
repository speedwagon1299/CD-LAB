%{

#include <stdio.h>
#include <stdlib.h>

%}

%token RECORD STATIC INT CHAR BOOL TRUE FALSE ID NUM LIT NL

%%

Program : RecordDecl NL {
    printf("\nValid");
    exit(0);
}
;
RecordDecl : RECORD ID '{' VarDeclList '}'
;
VarDeclList : VarDecl VarDeclList |
;
VarDecl : StaticOpt Type VarDefList ';'
;
StaticOpt : STATIC |
;
Type : INT | CHAR | BOOL
;
VarDefList : VarDef VarDefListTail
;
VarDefListTail : ',' VarDef VarDefListTail | 
;
VarDef : ID ArrayOpt InitOpt
;
ArrayOpt : '[' NUM ']' |
;
InitOpt : '=' Val | 
;
Val : TRUE | FALSE | NUM | LIT
;

%%

int yyerror(char* msg) {
    printf("\nInvalid");
    exit(1);
}

void yywrap() {

}

int main() {
    yyparse();
}