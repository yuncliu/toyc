%{
#include <stdlib.h>

#include "node.h"
#include "parser.h"
%}

%option nounput

%%

"if" {
return IF;
}

"print" {
return PRINT;
}

[a-z]+ {
    memset(yylval.var, 0x00, sizeof(yylval.var));
    strcpy(yylval.var, yytext);
    return VAR;
}

[0-9]+ {
    yylval.value = atof(yytext);
    return NUM;
}

[-+*/^()=] {
    return *yytext;
}

[\n] {
    return *yytext;
}

[;] {
    return *yytext;
}

[ \t\n]+    ;

%%
int yywrap() {
    return 1;
}
