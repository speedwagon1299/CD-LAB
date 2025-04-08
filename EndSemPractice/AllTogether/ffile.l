%{

#include "bfile.tab.h"
#include <stdio.h>
#include <stdlib.h>

%}

%%

[ \t]+ {

}

[\n] {
    return NL;
}

\"[^\"]*\"|\'[^\']*\' {
    return LIT;
}

"record"    { return RECORD; }

"static"    { return STATIC; }

"int"       { return INT; }

"char"      { return CHAR; }

"bool"      { return BOOL; }

"true"      { return TRUE; }

"false"     { return FALSE; }


[0-9]+ {
    return NUM;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    return ID;
}

. {
    return yytext[0];
}

%%