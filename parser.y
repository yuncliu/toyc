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
    NodeList* nodes;
}

%token <value> NUM
%token <str> VAR
%token IF WHILE PRINT
%left '-' '+'
%left '*' '/'
%right '^'
%type <node> exp
%type <nodes> lines

%%

input:
     %empty
    |input lines {
        LOG("input line\n");
        vector<Node*>::iterator it;
        for (it = $2->nodes.begin(); it != $2->nodes.end(); ++it) {
            printf("node [%d]\n", (*it)->operation);
            //(*it)->ex();
        }
        LOG("ex result = %f\n", $2->ex());
        $2->ex();
        print_symbol_table();
    }
;

lines:
    exp ';' {
        $$ = new NodeList();
        LOG("exp;\n");
        $$->push_back($1);
    }
    |lines exp ';' {
        $1->push_back($2);
        $$=$1;
    }
    |'{' lines '}' {
        printf("hellow {}\n");
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
        $$->addchild($1);
        $$->addchild($3);
    }
    |exp '-' exp {
        LOG("new node [-][%p]\n", $$);
        $$ = new Node(Node::opMINUS);
        $$->addchild($1);
        $$->addchild($3);
    }
    |exp '*' exp {
        LOG("new node [*][%p]\n", $$);
        $$ = new Node(Node::opMUL);
        $$->addchild($1);
        $$->addchild($3);
    }
    |exp '/' exp {
        LOG("new node [/][%p]\n", $$);
        $$ = new Node(Node::opDIV);
        $$->addchild($1);
        $$->addchild($3);
    }
    |exp '=' exp {
        LOG("new node [=][%p]\n", $$);
        $$ = new Node(Node::opASSIGN);
        $$->addchild($1);
        $$->addchild($3);
    }
    |'(' exp ')' {
        $$ = $2;
    }
    |IF '(' exp ')' lines {
        LOG("new [if] node\n");
        $$ = new Node(Node::opIF);
        $$->addchild($3);
        $$->nodelist = $5;
    }
    |PRINT '(' exp ')' {
        $$ = new Node(Node::opPRINT);
        $$->addchild($3);
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
