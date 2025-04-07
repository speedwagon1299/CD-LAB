%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

%}

%token dol_ID at_ID FUNC IF ELSE SUB NUM NL 
%left '+'
%left '-'
%left '*'
%left '/'

%%

prog : stmt_list NL NL {
    printf("\nValid Expression\n");
    exit(0);
}
;
stmt_list : stmt stmt_tail
;
stmt_tail : stmt stmt_tail | 
;
stmt : dol_ID '=' expr ';' | at_ID '=' '(' num_list ')' ';' | FUNC '(' ')' ';' | SUB FUNC block | IF '(' expr ')' block elseopt
;
elseopt : ELSE block | 
;
block : '{' stmt_list '}'
;
expr : term expr_tail
;
expr_tail : '+' term expr_tail | '-' term expr_tail | 
;
term : factor term_tail
;
term_tail : '*' factor term_tail | '/' factor term_tail | 
;
factor : dol_ID | NUM | '(' expr ')' | dol_ID '[' expr ']'
;
num_list : NUM num_list_tail
;
num_list_tail : ',' NUM num_list_tail | 
;


%%

int yyerror(char* msg) {
    printf("\nInvalid Expression\n");
    exit(1);
}

void yywrap() {
    
}

int main() {
    printf("\nEnter the expression:\n");
    yyparse();
    exit(0);
}