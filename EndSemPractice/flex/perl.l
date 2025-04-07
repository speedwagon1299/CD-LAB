%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

%}

%%

[ \t]+ {
    
}

[\n]+ {
    printf("\n");
}

[#].* {

}

"if"|"else"|"elsif"|"sub"|"return"|"while" {
    printf("<KEYWORD,%s>",yytext);
}

[0-9]+[.][0-9]+([eE][+-]?[0-9]+)? {
    printf("<FLOAT, %s>", yytext);
}

[0-9]+ {
    printf("<INT, %s>", yytext);
}


[$].* {
    printf("<$var,%s>",yytext);
}

[@].* {
    printf("<@var,%s>",yytext);
}

[+*/%-] {
    printf("<ARITH_OP,%c>",yytext[0]);
}

. {
    printf("<SPEC_SYMBOL,%c>", yytext[0]);
}

%%

int yywrap() {
    return 1;
}

int main() {
    yylex();
}