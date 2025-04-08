%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int row = 1, col = 1;

%}

%%

[ ]+ {
    col += strlen(yytext);
}

[\t]+ {
    col += 4*strlen(yytext);
}

[\n] {
    row++;
    col = 1;
    printf("\n");
}

[/][/].* {

}

"record"|"int"|"char"|"bool"|"true"|"false"|"static" {
    col += strlen(yytext);
    printf("<%d,%d,%s,%s>", row, col, "KW", yytext);
}

"\"[^\"]*\""|"\'[^\']*\'" {
    col += strlen(yytext);
    printf("<%d,%d,%s,%s>", row, col, "LIT", yytext);
}

[a-zA-Z_][a-zA-Z0-9_]* {
    col += strlen(yytext);
    printf("<%d,%d,%s,%s>", row, col, "ID", yytext);
}

[0-9]+\.[0-9]*([eE][+-]?[0-9]+)? {
    col += strlen(yytext);
    printf("<%d,%d,%s,%s>", row, col, "FLOAT", yytext);
}

[0-9]+ {
    col += strlen(yytext);
    printf("<%d,%d,%s,%s>", row, col, "NUM", yytext);
}

. {
    col++;
    printf("<%d,%d,%s,%c>", row, col, "SPEC", yytext[0]);
}

%%

int yywrap() {
    return 1;
}

int main() {
    printf("\nEnter the Expression:\n");
    yylex();
}