%{

#include "bfile.tab.h"

%}

%%

[ \t]+ {

}

[\n]+ {
    return NL;
}

"function"  {    return FUNCTION;    }
"int"       {    return INT;         }
"char"      {    return CHAR;        }
"bool"      {    return BOOL;        }
"true"      {    return TRUE;        }
"false"     {    return FALSE;       }
"if"        {    return IF;          }
"else"      {    return ELSE;        }

\"[^\"]*\"|\'[^\']*\' {
    return LIT;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    return ID;
}

[0-9]+ {
    return NUM;
}

. {
    return yytext[0];
}

%%