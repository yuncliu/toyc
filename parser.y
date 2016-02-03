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
extern char* yytext;
map<string, Node*> symbol_table;
%}

%union {
    double value;              /* integer value */
    char   str[50];            /* symbol table index */
    Node*  node;
}

%token <value> NUM
%token <str> VAR
%token IF PRINT
%left '-' '+'
%left '*' '/'
%right '^'
%type <node> exp
%type <node> line
%%

input:
     %empty
    |input line
;

line:
    '\n' {}
    |';' {printf("nothing\n");}
    |exp ';' {
        printf ("result = %f\n", $1->value);
        delete $1;
    }
    |PRINT '(' exp ')' ';' {
        printf("%f\n", $3->value);
        if ($3->type != Node::VARIABLES) {
            delete $3;
        }
    }
    |VAR '=' exp ';' {
        /*$1->value = $3->value;*/
        $$ = new Node($1, $3->value);
        delete $3;
    }
    |IF '(' exp ')' line{
        printf("fuck if\n");
        if ($3->value) {
            printf("fuck if true\n");
            $$ = $5;
        }
    }
    |'{' line '}' {
        $$ = $2;
    }
;

exp:
    NUM {
        $$ = new Node($1);
    }
    |VAR {
        map<string, Node*>::iterator it = symbol_table.find($1);
        if ( it != symbol_table.end()) {
            $$ = it->second;
        } else {
            $$ = new Node($1, 0);
        }
    }
    |exp '+' exp {
        $$ = new Node($1->value + $3->value);
        if ($1->type != Node::VARIABLES) {
            delete $1;
        }
        if ($3->type != Node::VARIABLES) {
            delete $3;
        }
    }
    |'(' exp ')' {
        $$ = $2;
    }
;

%%

void
yyerror (char const *s)
{
    fprintf (stderr, "%s\n", s);
}
