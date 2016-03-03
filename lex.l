%{
#include <stdlib.h>

#include "ast.h"
#include "Expr.h"
#include "Stmt.h"
#include "parser.h"
#if __cplusplus >= 201103L
    #define register
#endif
%}

%option nounput
%option noinput

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

"else" {
    return ELSE;
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

[0-9]+\.[0-9]* {
    return DOUBLE;
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
