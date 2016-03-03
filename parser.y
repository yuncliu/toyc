%{
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <string>
#include <map>
#include "ast.h"
#include "Expr.h"
#include "Stmt.h"
#include "Visitor.h"
using namespace std;
int yylex (void);
void yyerror (char const *);
extern char* yytext;
%}

%union {
    Expr*     expr;
    Stmt*       stmt;
    BlockAST*    blockast;
    FuncProtoType* funcprototype;
    FuncArgsAST* funcargsast;
    FuncCallArgs* callargs;
    Func*     funast;
    VarExpr*  varast;
    Type*        ty;
}

%token  DOUBLE
%token  INTEGER
%token  ID
%token IF ELSE WHILE PRINT
%token INTEGER_TYPE
%token DOUBLE_TYPE
%token RETURN
%left '-' '+'
%left '*' '/'
%right '^'
%type <expr> exp
%type <ty> type
%type <expr> identifier
%type <expr> var
%type <stmt> stmt
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
        Visitor* v = new Visitor();
        $1->Accept(v);
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
        $$->addStatement($1);
    }
    |stmt_list stmt {
        $$ = $1;
        $$->addStatement($2);
    }
;

stmt:
    exp ';' {
        $$ = new Stmt($1);
    }
    |var ';' {
        $$ = (Stmt*)new VarStmt($1);
    }
    |RETURN exp ';' {
        $$ = (Stmt*)new ReturnStmt($2);
    }
    |function {
        $$ = (Stmt*)$1;
    }
    | IF '(' exp ')' block {
        $$ = (Stmt*)new IfStmt($3, $5, NULL);
    }
    | IF '(' exp ')' block ELSE block {
        $$ = (Stmt*)new IfStmt($3, $5, $7);
    }
;

function:
    function_prototype block {
        $$ = new Func($1, $2);
    }
;

function_prototype:
    type identifier '(' function_args ')' {
        $$ = new FuncProtoType((IdExpr*)$2, $1, $4);
    }
;

function_args:
     {
        // for not args like  int foo()
        $$ = new FuncArgsAST();
    }
    |var {
        $$ = new FuncArgsAST();
        $$->addArg((VarExpr*)$1);
    }
    |function_args ',' var {
        $1->addArg((VarExpr*)$3);
    }
;

var:
    type identifier {
        $$ = (Expr*)new VarExpr($1, (IdExpr*)$2);
        printf("var define\n");
    }
;

exp:
    INTEGER {
        $$ = (Expr*)new IntExpr(atoi(yytext));
    }
    |DOUBLE {
        printf("new double [%f]\n", atof(yytext));
        $$ = (Expr*)new DoubleExpr(atof(yytext));
    }
    | exp '+' exp {
        $$ = (Expr*)new BinaryExpr('+', $1, $3);
    }
    | exp '-' exp {
        $$ = (Expr*)new BinaryExpr('-', $1, $3);
    }
    | exp '*' exp {
        $$ = (Expr*)new BinaryExpr('*', $1, $3);
    }
    | exp '/' exp {
        $$ = (Expr*)new BinaryExpr('/', $1, $3);
    }
    | exp '=' exp {
        $$ = (Expr*)new BinaryExpr('=', $1, $3);
    }
    | identifier {
        $$ = (Expr*)$1;
    }
    | var {
        $$ = $1;
    }
    | identifier '(' call_args ')' {
        printf("function call\n");
        $$ = (Expr*)new FuncCallExpr((IdExpr*)$1, $3);
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
        $$ = new IdExpr(yytext);
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
