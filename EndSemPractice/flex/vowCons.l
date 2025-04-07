%{

#include <stdio.h>
int c = 0;
int v = 0;

%}

%%

[aeiouAEIOU] {v++;}
[a-zA-Z] {c++;}

%%

int yywrap() {
	return 1;
}

int main() {
	yylex();
	printf("\nNumber of Vowels: %d", v);
	printf("\nNumber of Consonants: %d\n", c);
}
