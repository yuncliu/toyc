%{
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>
#include <map>
#include "ast.h"
using namespace std;
int yylex (void);
void yyerror (char const *);
extern char* yytext;
map<string, AST*> symbol_table;
void print_symbol_table();
%}

%union {
    double dvalue;              /* double value */
    int    ivalue;              /* integer value */
    char   str[50];            /* symbol table index */
    AST*        ast;
    VarAST*     varast;
    IdAST*      idast;
    TypeAST*    typeast;
}

%token <dvalue> DOUBLE
%token <ivalue> INTEGER
%token <str> ID
%type <str> DEF
%token DEF
%token IF WHILE PRINT
%token INTEGER_TYPE
%token DOUBLE_TYPE
%token RETURN
%left '-' '+'
%left '*' '/'
%right '^'
%type <ast> exp
%type <ast> lines
%type <ast> function
%type <ast> program
%type <typeast> type
%type <idast> identifier
%type <varast> var

%%
program:
    stmt_list {
        printf("proram: stmt_list\n");
        print_symbol_table();
    }
;

block:
    '{' stmt_list '}' {
    }
    |'{' '}' {
        printf("empty block\n");
    }
;

stmt_list:
    stmt {
        printf("statement\n");
    }
    |stmt_list stmt {
        printf("statement list\n");
    }
;

stmt:
    exp ';'
    |function {
        printf("stmt function\n");
    }
    |var ';'{
        printf("stmt var define\n");
    }
    |RETURN exp ';' {
        printf("return statement\n");
    }
;

function:
    type identifier '(' function_args ')' block {
        printf("get a function\n");
    }
;

function_args:
    |var {
        printf("one args\n");
    }
    |function_args ',' var {
        printf("add one args\n");
    }
;

var:
    type identifier {
        printf("var define\n");
    }
    |type identifier '=' exp {
        printf("var define and assignmeng\n");
        printf("xxxxxxxxx %s\n", ((TypeAST*)($1))->type.c_str());
        printf("xxxxxxxxx %s\n", $2->name.c_str());
        $$ = new VarAST($1, $2, $4);
        symbol_table.insert(pair<string, AST*>($2->name, (AST*)$$));
    }
;

exp:
    INTEGER {
        printf("integer\n");
    }
;

identifier:
  ID {
        $$ = new IdAST(yytext);
        printf("identifier %s\n", yytext);
  }
;

type:
    DOUBLE_TYPE {
        $$ = new TypeAST("double");
    }
    |INTEGER_TYPE {
        $$ = new TypeAST("int");
        printf("%s\n", "type int");
    }
;
%%

void
yyerror (char const *s)
{
    fprintf (stderr, "%s\n", s);
}

void print_symbol_table() {
    map<string, AST*>::iterator it;
    printf("symbol table:\n");
    for (it = symbol_table.begin(); it != symbol_table.end(); ++it) {
        printf("symbol [%s]\n", it->first.c_str());
    }
}
