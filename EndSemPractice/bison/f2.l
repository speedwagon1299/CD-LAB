%{

#include "b2.tab.h"

%}

%%

[ \t]+ {

}

[\n]+ {
    return NL;
}

[#].* {
    
}

"if" {
    return IF;
}

"else" {
    return ELSE;
}

"sub" {
    return SUB;
}

[0-9]+ {
    return NUM;
}

[$][a-zA-Z_][a-zA-Z0-9_]* {
    return dol_ID;
}

[@][a-zA-Z_][a-zA-Z0-9_]* {
    return at_ID;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    return FUNC;
}

. {
    return yytext[0];
}

%%
