%{
#include <stdlib.h>

#include "node.h"
#include "parser.h"
%}

%option nounput

%%

[0-9]+ {
    yylval.value = atof(yytext);
    return NUM;
}
[-+*/^()] {
    return *yytext;
}

[\n] {
    return *yytext;
}


%%
int yywrap() {
    return 1;
}
