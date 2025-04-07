%{

#include "bfile.tab.h"

%}

%%

[ \t]+ {

}

[\n]+ {
    return NL;
}

("int"|"char"|"double"|"float") {
    return DTYPE;
}

[0-9]+ {
    return NUM;
}

[a-zA-Z_]+[a-zA-Z0-9_]* {
    return ID;
}

. {
    return yytext[0];
}

%%

