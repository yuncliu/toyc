%{
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <string>
#include <map>
//#include "ast.h"
#include "Expr.h"
#include "Stmt.h"
#include "DumpVisitor.h"
#include "Visitor.h"
using namespace std;
int yylex (void);
void yyerror (char const *);
extern char* yytext;
extern Stmt* root;
%}

%union {
    Stmt*       stmt;
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
%type <stmt> exp
%type <stmt> type
%type <stmt> identifier
%type <stmt> var
%type <stmt> stmt
%type <stmt> stmt_list
%type <stmt> block
%type <stmt> function_prototype
%type <stmt> function_args
%type <stmt> call_args
%type <stmt> function
%%
program:
    stmt_list {
        root = $1;
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
        CompoundStmt* p =  new CompoundStmt();
        p->addStatement($1);
        $$ = (Stmt*)p;
    }
    |stmt_list stmt {
        CompoundStmt* p =  (CompoundStmt*)$1;
        p->addStatement($2);
        $$ = $1;
    }
;

stmt:
    exp ';' {
        $$ = $1;
    }
    |var ';' {
        $$ = $1;
    }
    |RETURN exp ';' {
        $$ = (Stmt*)new ReturnStmt($2);
    }
    |function {
        $$ = (Stmt*)$1;
    }
    | IF '(' exp ')' block {
        $$ = (Stmt*)new IfStmt($3, (CompoundStmt*)$5, NULL);
    }
    | IF '(' exp ')' block ELSE block {
        $$ = (Stmt*)new IfStmt($3, (CompoundStmt*)$5, (CompoundStmt*)$7);
    }
;

function:
    function_prototype block {
        $$ = (Stmt*)new Func((FuncProtoType*)$1, (CompoundStmt*)$2);
    }
;

function_prototype:
    type identifier '(' function_args ')' {
        $$ = (Stmt*)new FuncProtoType((IdExpr*)$2, (TypeExpr*)$1, (FuncParameter*)$4);
    }
;

function_args:
     {
        // for not args like  int foo()
        $$ = (Stmt*)new FuncParameter();
    }
    |var {
        FuncParameter* p = new FuncParameter();
        p->addParam($1);
        $$ = (Stmt*)p;
    }
    |function_args ',' var {
        FuncParameter* p = (FuncParameter*)$1;
        p->addParam($3);
        $$ = $1;
    }
;

var:
    type identifier {
        $$ = (Stmt*)new VarExpr((TypeExpr*)$1, (IdExpr*)$2);
    }
;

exp:
    INTEGER {
        $$ = (Stmt*)new IntExpr(atoi(yytext));
    }
    |DOUBLE {
        printf("new double [%f]\n", atof(yytext));
        $$ = (Stmt*)new DoubleExpr(atof(yytext));
    }
    | exp '+' exp {
        $$ = (Stmt*)new BinaryExpr('+', $1, $3);
    }
    | exp '-' exp {
        $$ = (Stmt*)new BinaryExpr('-', $1, $3);
    }
    | exp '*' exp {
        $$ = (Stmt*)new BinaryExpr('*', $1, $3);
    }
    | exp '/' exp {
        $$ = (Stmt*)new BinaryExpr('/', $1, $3);
    }
    | exp '=' exp {
        $$ = (Stmt*)new BinaryExpr('=', $1, $3);
    }
    | identifier {
        $$ = (Stmt*)$1;
    }
    | var {
        $$ = $1;
    }
    | identifier '(' call_args ')' {
        printf("function call\n");
        $$ = (Stmt*)new FuncCallExpr((IdExpr*)$1, (FuncCallParams*)$3);
    }
;

call_args:
     {
        $$ = (Stmt*)new FuncCallParams();
    }
    | exp {
        FuncCallParams* p = new FuncCallParams();
        p->pushParam($1);
        $$ = (Stmt*)p;
    }
    | call_args ',' exp{
        FuncCallParams* p = (FuncCallParams*)$1;
        p->pushParam($3);
        $$ = (Stmt*)p;
    }
;

identifier:
    ID {
        $$ = (Stmt*)new IdExpr(yytext);
    }
;

type:
    DOUBLE_TYPE {
        $$ = (Stmt*)new TypeExpr("double");
    }
    |INTEGER_TYPE {
        $$ = (Stmt*)new TypeExpr("int");
    }
;
%%

void
yyerror (char const *s)
{
    fprintf (stderr, "%s\n", s);
}
