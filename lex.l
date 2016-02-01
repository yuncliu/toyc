%{
#include "parser.h"
#include <stdlib.h>
%}

%option nounput

%%

[0-9]+ {
    yylval = atoi(yytext);
    printf("NUM\n");
    return NUM;
}
[-+*/^()] {
    printf("OP\n");
    return *yytext;
}

[\n] {
    return *yytext;
}


%%
int yywrap() {
    return 1;
}
