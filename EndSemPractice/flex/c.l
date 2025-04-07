%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

%}

num [0-9]+

%%

"//".* {

}

"/*"[^/\*]"*/" {

}

[ \t]+ {

}

[\n]+ {
    printf("\n");
}

\"[^\"]*\" {
    printf("<STRING_LITERAL,%s>", yytext);
}

("if"|"else"|"void"|"int"|"float"|"char"|"return"|"for"|"while") {
    printf("<KEYWORD,%s>", yytext);
}

[a-zA-Z_][a-zA-Z0-9_]* {
    printf("<IDENTIFIER,%s>", yytext);
}

[+-]?({num})[.]({num})([eE][+-]?({num}))? {
    printf("<FLOAT,%s>",yytext);
}

[+-]?({num}) {
    printf("<NUMBER,%s>",yytext);
}

[=] {
    printf("<ASSIGN_OP,%c>", yytext[0]);
}

[+*/%-] {
    printf("<ARITH_OP,%c>", yytext[0]);
}

("=="|"!="|"<"|"<="|">"|">=") {
    printf("<REL_OP,%s>", yytext);
}

((&)|(\|)|(&&)|(\|\|)) {
    printf("<LOG_OP,%s>", yytext);
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