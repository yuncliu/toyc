%{
#include <stdio.h>
#include <math.h>
int yylex (void);
void yyerror (char const *);
%}

%define api.value.type {int}
%token NUM
%left '-' '+'
%left '*' '/'
%right '^'
%precedence NEG

%%
input:
    |input line
;

line:
    '\n'
    | exp '\n' { printf ("result = %d\n", $1); }
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
