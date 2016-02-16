%{
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <string>
#include <map>
#include "ast.h"
using namespace std;
int yylex (void);
void yyerror (char const *);
extern char* yytext;
map<string, IdAST*> symbol_table;
void print_symbol_table();
%}

%union {
    double dvalue;              /* double value */
    int    ivalue;              /* integer value */
    char   str[50];            /* symbol table index */
    AST*         ast;
    IdAST*       idast;
    ExprAST*     exprast;
    StmtAST*     stmtast;
    StmtlistAST* stmtlistast;
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
%type <exprast> exp
%type <ast> function
%type <ast> program
%type <str> type
%type <idast> identifier
%type <idast> var
%type <stmtast> stmt
%type <stmtlistast> stmt_list
%type <stmtlistast> block
%%
program:
    stmt_list {
        printf("proram: stmt_list\n");
        $1->codegen();
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
        $$ = new StmtlistAST();
        $$->stmts.push_back($1);
    }
    |stmt_list stmt {
        $1->stmts.push_back($2);
        $$ = $1;
        printf("statement list\n");
    }
;

stmt:
    exp ';'
    {
        $$ = new StmtAST();
    }
    |function {
        $$ = new StmtAST();
        printf("stmt function\n");
    }
    |var ';'{
        $$ = new StmtAST();
        $$->value = (AST*)$1;
        printf("stmt var define\n");
    }
    |RETURN exp ';' {
        $$ = new StmtAST();
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
        $$ = $2;
        $$->type = string($1);
        $$->value = (AST*)$4;
        symbol_table.insert(pair<string, IdAST*>($2->name, $$));
    }
;

exp:
    INTEGER {
        printf("integer\n");
        $$ = new ExprAST();
        $$->value = atoi(yytext);
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
        strcpy($$, "double");
    }
    |INTEGER_TYPE {
        strcpy($$, "int");
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
    map<string, IdAST*>::iterator it;
    printf("symbol table:\n");
    for (it = symbol_table.begin(); it != symbol_table.end(); ++it) {
        printf("symbol [%s] type [%s]\n",
            it->second->name.c_str(),
            it->second->type.c_str());
    }
}
