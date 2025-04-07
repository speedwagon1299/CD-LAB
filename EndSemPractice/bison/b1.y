%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

%}

%token DTYPE ID NUM NL

%%

stmt: decl NL {
    printf("\nValid Expression\n");
    exit(0);
}
;
decl: DTYPE iden more_id ';' decl | 
;
iden: ID | ID '['NUM']' 
;
more_id: ',' iden more_id | 
;

%%

int yyerror(char* msg) {
    printf("\nInvalid Expression\n");
    exit(1);
}

int yywrap() {
    return 1;
}

int main() {
    printf("\nEnter the expression:\n");
    yyparse();
    return 0;
}