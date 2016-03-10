%{
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <string>
#include <map>
#include <memory>
#include "Expr.h"
#include "Stmt.h"
#include "DumpVisitor.h"
#include "Visitor.h"
using namespace std;
int yylex (void);
void yyerror (char const *);
extern char* yytext;
//extern Stmt* root;
//extern std::weak_ptr<Stmt> root;
extern std::shared_ptr<Stmt> root;
#define YYSTYPE std::shared_ptr<Stmt>
%}

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
        shared_ptr<CompoundStmt> p(new CompoundStmt());
        p->addStatement($1);
        $$ = p;
    }
    |stmt_list stmt {
        shared_ptr<CompoundStmt> p = std::static_pointer_cast<CompoundStmt>($1);
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
        $$ = shared_ptr<Stmt>(new ReturnStmt($2));
    }
    |function {
        $$ = $1;
    }
    | IF '(' exp ')' block {
        $$ = shared_ptr<Stmt>(new IfStmt($3, $5));
    }
    | IF '(' exp ')' block ELSE block {
        $$ = shared_ptr<Stmt>(new IfStmt($3, $5, $7));
    }
;

function:
    function_prototype block {
        shared_ptr<FuncProtoType> p1 = std::static_pointer_cast<FuncProtoType>($1);
        shared_ptr<CompoundStmt> p2 = std::static_pointer_cast<CompoundStmt>($2);
        $$ = shared_ptr<Stmt>(new Func(p1, p2));
    }
;

function_prototype:
    type identifier '(' function_args ')' {
        shared_ptr<IdExpr> p1 = std::static_pointer_cast<IdExpr>($2);
        shared_ptr<TypeExpr> p2 = std::static_pointer_cast<TypeExpr>($1);
        shared_ptr<FuncParameter> p3 = std::static_pointer_cast<FuncParameter>($4);
        $$ = shared_ptr<Stmt>(new FuncProtoType(p1, p2, p3));
    }
;

function_args:
     {
        $$ = shared_ptr<Stmt>(new FuncParameter());
    }
    |var {
        printf("xxxxxxxx\n");
        std::shared_ptr<FuncParameter> p(new FuncParameter());
        p->addParam($1);
        $$ = p;
    }
    |function_args ',' var {
        std::shared_ptr<FuncParameter> p = std::static_pointer_cast<FuncParameter>($1);
        p->addParam($3);
        $$ = $1;
    }
;

var:
    type identifier {
        std::shared_ptr<TypeExpr> p1 = std::static_pointer_cast<TypeExpr>($1);
        std::shared_ptr<IdExpr> p2 = std::static_pointer_cast<IdExpr>($2);
        $$ = shared_ptr<Stmt>(new VarExpr(p1, p2));
    }
;

exp:
    INTEGER {
        $$ = shared_ptr<Stmt>(new IntExpr(atoi(yytext)));
    }
    |DOUBLE {
        printf("new double [%f]\n", atof(yytext));
        $$ = shared_ptr<Stmt>(new DoubleExpr(atof(yytext)));
    }
    | exp '+' exp {
        $$ = shared_ptr<Stmt>(new BinaryExpr('+', $1, $3));
    }
    | exp '-' exp {
        $$ = shared_ptr<Stmt>(new BinaryExpr('-', $1, $3));
    }
    | exp '*' exp {
        $$ = shared_ptr<Stmt>(new BinaryExpr('*', $1, $3));
    }
    | exp '/' exp {
        $$ = shared_ptr<Stmt>(new BinaryExpr('/', $1, $3));
    }
    | exp '=' exp {
        $$ = shared_ptr<Stmt>(new BinaryExpr('=', $1, $3));
    }
    | identifier {
        $$ = $1;
    }
    | var {
        $$ = $1;
    }
    | identifier '(' call_args ')' {
        printf("function call\n");
        std::shared_ptr<IdExpr> p1 = std::static_pointer_cast<IdExpr>($1);
        std::shared_ptr<FuncCallParams> p2= std::static_pointer_cast<FuncCallParams>($3);
        $$ = shared_ptr<Stmt>(new FuncCallExpr(p1, p2));
    }
;

call_args:
     {
        $$ = shared_ptr<Stmt>(new FuncCallParams());
    }
    | exp {
        std::shared_ptr<FuncCallParams> p(new FuncCallParams());
        p->pushParam($1);
        $$ = p;
    }
    | call_args ',' exp{
        std::shared_ptr<FuncCallParams> p = std::static_pointer_cast<FuncCallParams>($1);
        p->pushParam($3);
        $$ = $1;
    }
;

identifier:
    ID {
        $$ = shared_ptr<Stmt>(new IdExpr(yytext));
    }
;

type:
    DOUBLE_TYPE {
        $$ = shared_ptr<Stmt>(new TypeExpr("double"));
    }
    |INTEGER_TYPE {
        $$ = shared_ptr<Stmt>(new TypeExpr("int"));
    }
;
%%

void
yyerror (char const *s)
{
    fprintf (stderr, "%s\n", s);
}
