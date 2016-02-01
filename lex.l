%{
#include "parser.h"
#include <stdlib.h>
%}

%option nounput

%%

[0-9]+ {
    yylval = atoi(yytext);
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
