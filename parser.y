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
%}

%union {
    double dvalue;              /* double value */
    int    ivalue;              /* integer value */
    char   str[50];            /* symbol table index */
    ExprAST*     exprast;
    StmtAST*     stmtast;
    BlockAST*    blockast;
    FuncAST*     funast;
    FuncArgsAST*     funcargsast;
    VarExprAST*      varast;
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
%type <funast> function
%type <ast> program
%type <str> type
%type <exprast> identifier
%type <exprast> var
%type <stmtast> stmt
%type <blockast> stmt_list
%type <blockast> block
%type <funcargsast> function_args
%%
program:
    stmt_list {
        printf("proram: stmt_list\n");
        $1->codegen();
    }
;

block:
    '{' stmt_list '}' {
        $$ = $2;
    }
    |'{' '}' {
        printf("empty block\n");
    }
;

stmt_list:
    stmt {
        $$ = new BlockAST();
        $$->stmts.push_back($1);
    }
    |stmt_list stmt {
        $1->stmts.push_back($2);
        $$ = $1;
    }
;

stmt:
    exp ';' {
        $$ = new StmtAST($1);
    }
    |var ';' {
        $$ = (StmtAST*)new VarStmtAST($1);
    }
    |RETURN exp ';' {
        $$ = (StmtAST*)new ReturnStmtAST($2);
    }
    |function {
        $$ = (StmtAST*)$1;
    }
;

function:
    type identifier '(' function_args ')' block {
        printf("get a function\n");
        $$ = new FuncAST(((IdExprAST*)$2)->name);
        $$->functype->arg_list = $4;
        $$->body = $6;
    }
;

function_args:
     {
        // for not args like  int foo()
        $$ = new FuncArgsAST();
    }
    |var {
        $$ = new FuncArgsAST();
        VarExprAST* p = (VarExprAST*)$1;
        $$->addarg(p->name, p->type);
        printf("one args\n");
    }
    |function_args ',' var {
        VarExprAST* p = (VarExprAST*)$3;
        $1->addarg(p->name, p->type);
        printf("add one args\n");
    }
;

var:
    type identifier {
        $$ = (ExprAST*)new VarExprAST($1, ((IdExprAST*)$2)->name);
        printf("var define\n");
    }
    |type identifier '=' exp {
        printf("var define and assignmeng\n");
    }
;

exp:
    INTEGER {
        $$ = (ExprAST*)new IntExprAST(atoi(yytext));
    }
    | exp '+' exp {
        $$ = (ExprAST*)new BinaryExprAST('+', $1, $3);
    }
    | exp '=' exp {
        $$ = (ExprAST*)new BinaryExprAST('=', $1, $3);
    }
    | identifier {
        $$ = (ExprAST*)$1;
    }
;

identifier:
  ID {
        $$ = new IdExprAST(yytext);
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
