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
%token IF WHILE PRINT
%left '-' '+'
%left '*' '/'
%right '^'
%type <node> exp
%type <node> lines

%%

input:
     %empty
    |input lines {
        LOG("input line\n");
        vector<Node*>::iterator it;
        for (it = $2->nodes.begin(); it != $2->nodes.end(); ++it) {
            LOG("node [%d]\n", (*it)->operation);
        }
        double r = $2->ex();
        LOG("ex result = %f\n", r);
        delete $2;
        print_symbol_table();
    }
;

lines:
    exp ';' {
        $$ = new Node(Node::opLIST);
        LOG("exp;\n");
        $$->push_parameter($1);
    }
    |lines exp ';' {
        $1->push_parameter($2);
        $$=$1;
    }
    |'{' lines '}' {
        $$ = $2;
    }
;

exp:
    NUM {
        LOG("new num node[%f]\n", $1);
        $$ = new Node($1);
    }
    |VAR {
        map<string, Node*>::iterator it = symbol_table.find($1);
        if ( it != symbol_table.end()) {
            $$ = it->second;
            LOG("use var node[%s] [%f]\n", $1, $$->value);
        } else {
            LOG("new var node[%s]\n", $1);
            $$ = new Node($1, 0);
        }
    }
    |exp '+' exp {
        LOG("new node [+][%p]\n", $$);
        $$ = new Node(Node::opADD);
        $$->push_parameter($1);
        $$->push_parameter($3);
    }
    |exp '-' exp {
        LOG("new node [-][%p]\n", $$);
        $$ = new Node(Node::opMINUS);
        $$->push_parameter($1);
        $$->push_parameter($3);
    }
    |exp '*' exp {
        LOG("new node [*][%p]\n", $$);
        $$ = new Node(Node::opMUL);
        $$->push_parameter($1);
        $$->push_parameter($3);
    }
    |exp '/' exp {
        LOG("new node [/][%p]\n", $$);
        $$ = new Node(Node::opDIV);
        $$->push_parameter($1);
        $$->push_parameter($3);
    }
    |exp '=' exp {
        LOG("new node [=][%p]\n", $$);
        $$ = new Node(Node::opASSIGN);
        $$->push_parameter($1);
        $$->push_parameter($3);
    }
    |'(' exp ')' {
        $$ = $2;
    }
    |IF '(' exp ')' lines {
        LOG("new [if] node\n");
        $$ = new Node(Node::opIF);
        $$->push_parameter($3);
        $$->push_parameter($5);
    }
    |WHILE '(' exp ')' lines {
        LOG("new [while] node\n");
        $$ = new Node(Node::opWHILE);
        $$->push_parameter($3);
        $$->push_parameter($5);
    }
    |PRINT '(' exp ')' {
        $$ = new Node(Node::opPRINT);
        $$->push_parameter($3);
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
    LOG("symbol table:\n");
    for (it = symbol_table.begin(); it != symbol_table.end(); ++it) {
        LOG("symbol [%s] value [%f]\n", it->first.c_str(), it->second->value);
    }
}
