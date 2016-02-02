%{
#include <stdio.h>
#include <math.h>
#include "node.h"
int yylex (void);
void yyerror (char const *);
%}

%union {
    double value;              /* integer value */
    char index;                /* symbol table index */
    nodeType *nPtr;            /* node pointer */
}

%token <value> NUM
%token <index> VAR
%left '-' '+'
%left '*' '/'
%right '^'
%type <value> exp
%%
input:
    |input line
;

line:
    '\n'
    | exp '\n' { printf ("result = %f\n", $1); }
;
exp:
    NUM { $$ = $1; }
    |exp '+' exp { $$ = $1 + $3; }
    |exp '-' exp { $$ = $1 - $3; }
    |exp '*' exp { $$ = $1 * $3; }
    |exp '/' exp { $$ = $1 / $3; }
    |'-' exp     { $$ = -$2; }
    |exp '^' exp { $$ = pow ($1 , $3); }
    |'(' exp ')' { $$ = $2; }
;
%%

void
yyerror (char const *s)
{
    fprintf (stderr, "%s\n", s);
}
