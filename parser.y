%{
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <string>
#include <map>
#include <memory>
//#include "ast.h"
#include "Expr.h"
#include "Stmt.h"
#include "DumpVisitor.h"
#include "Visitor.h"
using namespace std;
int yylex (void);
void yyerror (char const *);
extern char* yytext;
//extern Stmt* root;
extern std::weak_ptr<Stmt> root;
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
        //CompoundStmt* p =  new CompoundStmt();
        shared_ptr<CompoundStmt> p(new CompoundStmt());
        p->addStatement($1);
        $$ = p;
    }
    |stmt_list stmt {
        //CompoundStmt* p =  (CompoundStmt*)$1;
        $1->addStatement($2);
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
        //$$ = (Stmt*)new ReturnStmt($2);
        $$ = shared_ptr<Stmt>(new ReturnStmt($2));
    }
    |function {
        //$$ = (Stmt*)$1;
        $$ = $1;
    }
    | IF '(' exp ')' block {
        //$$ = (Stmt*)new IfStmt($3, (CompoundStmt*)$5, NULL);
        $$ = shared_ptr<Stmt>(new IfStmt($3, $5));
    }
    | IF '(' exp ')' block ELSE block {
        //$$ = (Stmt*)new IfStmt($3, (CompoundStmt*)$5, (CompoundStmt*)$7);
        $$ = shared_ptr<Stmt>(new IfStmt($3, $5, $7));
    }
;

function:
    function_prototype block {
        //$$ = (Stmt*)new Func((FuncProtoType*)$1, (CompoundStmt*)$2);
        shared_ptr<FuncProtoType> p1 = std::static_pointer_cast<FuncProtoType>($1);
        shared_ptr<CompoundStmt> p2 = std::static_pointer_cast<CompoundStmt>($2);
        $$ = shared_ptr<Stmt>(new Func(p1, p2));
    }
;

function_prototype:
    type identifier '(' function_args ')' {
        //$$ = (Stmt*)new FuncProtoType((IdExpr*)$2, (TypeExpr*)$1, (FuncParameter*)$4);
        shared_ptr<IdExpr> p1 = std::static_pointer_cast<IdExpr>($2);
        shared_ptr<TypeExpr> p2 = std::static_pointer_cast<TypeExpr>($1);
        shared_ptr<FuncParameter> p3 = std::static_pointer_cast<FuncParameter>($4);
        $$ = shared_ptr<Stmt>(new FuncProtoType(p1, p2, p3));
    }
;

function_args:
     {
        // for not args like  int foo()
        //$$ = (Stmt*)new FuncParameter();
        $$ = shared_ptr<Stmt>(new FuncParameter());
    }
    |var {
        //FuncParameter* p = new FuncParameter();
        std::shared_ptr<FuncParameter> p(new FuncParameter());
        p->addParam($1);
        $$ = std::static_pointer_cast<FuncParameter>(p);
    }
    |function_args ',' var {
        std::shared_ptr<FuncParameter> p = std::static_pointer_cast<FuncParameter>($1);
        p->addParam($3);
        $$ = $1;
    }
;

var:
    type identifier {
        //$$ = (Stmt*)new VarExpr((TypeExpr*)$1, (IdExpr*)$2);
        std::shared_ptr<TypeExpr> p1 = std::static_pointer_cast<TypeExpr>($1);
        std::shared_ptr<IdExpr> p2 = std::static_pointer_cast<IdExpr>($2);
        $$ = shared_ptr<Stmt>(new VarExpr(p1, p2));
    }
;

exp:
    INTEGER {
        //$$ = (Stmt*)new IntExpr(atoi(yytext));
        $$ = shared_ptr<Stmt>(new IntExpr(atoi(yytext)));
    }
    |DOUBLE {
        printf("new double [%f]\n", atof(yytext));
        //$$ = (Stmt*)new DoubleExpr(atof(yytext));
        $$ = shared_ptr<Stmt>(new DoubleExpr(atof(yytext)));
    }
    | exp '+' exp {
        //$$ = (Stmt*)new BinaryExpr('+', $1, $3);
        $$ = shared_ptr<Stmt>(new BinaryExpr('+', $1, $3));
    }
    | exp '-' exp {
        //$$ = (Stmt*)new BinaryExpr('-', $1, $3);
        $$ = shared_ptr<Stmt>(new BinaryExpr('-', $1, $3));
    }
    | exp '*' exp {
        //$$ = (Stmt*)new BinaryExpr('*', $1, $3);
        $$ = shared_ptr<Stmt>(new BinaryExpr('*', $1, $3));
    }
    | exp '/' exp {
        //$$ = (Stmt*)new BinaryExpr('/', $1, $3);
        $$ = shared_ptr<Stmt>(new BinaryExpr('/', $1, $3));
    }
    | exp '=' exp {
        //$$ = (Stmt*)new BinaryExpr('=', $1, $3);
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
        //$$ = (Stmt*)new FuncCallExpr((IdExpr*)$1, (FuncCallParams*)$3);
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
        $$ = p;
        //$$ = std::static_pointer_cast<FuncCallParams>(p);
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
