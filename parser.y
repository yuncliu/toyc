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
void print_symbol_table();
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
%type <node> lines

%%

input:
     %empty
    |input line {
        printf("input line\n");
        $2->ex();
        printf("ex result = %f\n", $2->value);
        print_symbol_table();
    }
;

line:
    ';' {
        printf("nothing\n");
        $$= new Node(Node::opSEMICOLON);
    }
    |exp ';' {
        printf("exp;\n");
        $$ = $1;
    }
    |IF '(' exp ')' exp {
        $$ = new Node(Node::opIF);
        $$->addchild($3);
        $$->addchild($5);
    }
    |'{' lines '}' {
        $$ = $2;
    }
;

lines:
     line        {$$=$1;}
    |lines line {
        printf("new ; node\n");
        $$ = new Node(Node::opSEMICOLON);
        $$->addchild($1);
        $$->addchild($2);
    }
;

exp:
    NUM {
        printf("new num node[%f]\n", $1);
        $$ = new Node($1);
    }
    |VAR {
        map<string, Node*>::iterator it = symbol_table.find($1);
        if ( it != symbol_table.end()) {
            $$ = it->second;
            printf("use var node[%s] [%f]\n", $1, $$->value);
        } else {
            printf("new var node[%s]\n", $1);
            $$ = new Node($1, 0);
        }
    }
    |exp '+' exp {
        printf("new add node [+][%p]\n", $$);
        $$ = new Node(Node::opADD);
        $$->addchild($1);
        $$->addchild($3);
    }
    |exp '=' exp {
        printf("new add node [=][%p]\n", $$);
        $$ = new Node(Node::opASSIGN);
        $$->addchild($1);
        $$->addchild($3);
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

void print_symbol_table() {
    map<string, Node*>::iterator it;
    printf("symbol table:\n");
    for (it = symbol_table.begin(); it != symbol_table.end(); ++it) {
        printf("symbol [%s] value [%f]\n", it->first.c_str(), it->second->value);
    }
}
