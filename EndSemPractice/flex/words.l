%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int words = 0, chars = 0, blanks = 0, lines = 0;

%}

%%

[a-zA-Z]+ {
    words++;
    chars += strlen(yytext);
}

[\n] {
    lines++;
}

[ ] {
    blanks++;
}

. {
    chars++;
}

%%

int yywrap() {
    return 1;
}

int main() {
    yylex();
    printf("\nWords: %d", words);
    printf("\nCharacters: %d", chars);
    printf("\nBlanks: %d", blanks);
    printf("\nLines: %d", lines);
}