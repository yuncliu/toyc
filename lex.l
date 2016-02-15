%{
#include <stdlib.h>

#include "ast.h"
#include "parser.h"
%}

%option nounput

%%

"double" {
    return DOUBLE_TYPE;
}

"int" {
    return INTEGER_TYPE;
}

"return" {
    return RETURN;
}

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
    return ID;
}

[0-9]+ {
    return INTEGER;
}

[-+*/^()={},] {
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
