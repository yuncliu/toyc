%{
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>
#include <map>
#include "node.h"
using namespace std;
int yylex (void);
void yyerror (char const *);
map<string, double> symbol_table;
extern char* yytext;
%}

%union {
    double value;              /* integer value */
    char   var[20];                /* symbol table index */
}

%token <value> NUM
%token <index> VAR
%token IF PRINT
%left '-' '+'
%left '*' '/'
%right '^'
%type <value> exp
%type <value> line
%%

input:
     %empty
    |input line
;
line:
    '\n'
    | exp ';' { printf ("result = %f\n", $1); }
    | PRINT exp ';' { printf("%f\n", $2); }
;

exp:
    NUM { $$ = $1; }
    |VAR {
        map<string, double>::iterator it;
        for (it = symbol_table.begin(); it != symbol_table.end(); ++it) {
            printf("%s -> %f\n", it->first.c_str(), it->second);
        }
        it = symbol_table.find(yyval.var);
        if (it != symbol_table.end()) {
            $$ = it->second;
        }
    }
    |VAR '=' exp { symbol_table[yyval.var] = $3; $$ = $3;}
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
