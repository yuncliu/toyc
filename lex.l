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

"while" {
    return WHILE;
}

"print" {
    return PRINT;
}

[a-z]+ {
    memset(yylval.str, 0x00, sizeof(yylval.str));
    strcpy(yylval.str, yytext);
    return VAR;
}

[0-9]+ {
    yylval.value = atof(yytext);
    return NUM;
}

[-+*/^()=] {
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
