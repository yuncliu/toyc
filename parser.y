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
    ExprAST*     exprast;
    StmtAST*     stmtast;
    BlockAST*    blockast;
    FuncProtoType* funcprototype;
    FuncArgsAST* funcargsast;
    FuncCallArgs* callargs;
    FuncAST*     funast;
    VarExprAST*  varast;
    Type*        ty;
}

%token  DOUBLE
%token  INTEGER
%token  ID
%token IF WHILE PRINT
%token INTEGER_TYPE
%token DOUBLE_TYPE
%token RETURN
%left '-' '+'
%left '*' '/'
%right '^'
%type <exprast> exp
%type <ty> type
%type <exprast> identifier
%type <exprast> var
%type <stmtast> stmt
%type <blockast> stmt_list
%type <blockast> block
%type <funcprototype> function_prototype
%type <funcargsast> function_args
%type <callargs> call_args
%type <funast> function
%%
program:
    stmt_list {
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
        $$ = (StmtAST*)new VarStmtAST((VarExprAST*)$1);
    }
    |RETURN exp ';' {
        $$ = (StmtAST*)new ReturnStmtAST($2);
    }
    |function {
        $$ = (StmtAST*)$1;
    }
;

function:
    function_prototype block {
        $$ = new FuncAST($1, $2);
    }
;

function_prototype:
    type identifier '(' function_args ')' {
        $$ = new FuncProtoType((IdExprAST*)$2, $1, $4);
    }
;

function_args:
     {
        // for not args like  int foo()
        $$ = new FuncArgsAST();
    }
    |var {
        $$ = new FuncArgsAST();
        $$->addarg((VarExprAST*)$1);
    }
    |function_args ',' var {
        $1->addarg((VarExprAST*)$3);
    }
;

var:
    type identifier {
        $$ = (ExprAST*)new VarExprAST($1, (IdExprAST*)$2);
        printf("var define\n");
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
    | var {
        $$ = $1;
    }
    | identifier '(' call_args ')' {
        printf("function call\n");
        $$ = (ExprAST*)new FuncCallExpr((IdExprAST*)$1, $3);
    }
;

call_args:
     {
        $$ = new FuncCallArgs();
    }
    | exp {
        $$ = new FuncCallArgs();
        $$->pushArg($1);
    }
    | call_args ',' exp{
        $$ = $1;
        $$->pushArg($3);
    }
;
identifier:
  ID {
        $$ = new IdExprAST(yytext);
  }
;

type:
    DOUBLE_TYPE {
        $$ = Type::getDoubleTy(getGlobalContext());
    }
    |INTEGER_TYPE {
        $$ = Type::getInt32Ty(getGlobalContext());
    }
;
%%

void
yyerror (char const *s)
{
    fprintf (stderr, "%s\n", s);
}
