#include "Visitor.h"
#include "ast.h"
#include "Expr.h"
#include "Stmt.h"

Visitor::Visitor():indent(0) {
}

Visitor::~Visitor() {
}

void Visitor::print_indent() {
    for (int i = 0; i < this->indent; ++i) {
        printf(" ");
    }
}

void Visitor::VisitExpr(Expr* expr) {
}

void Visitor::VisitStmt(Stmt* stmt) {
    print_indent();
    printf("%s\n", stmt->Info().c_str());
}

void Visitor::VisitFunc(Func* func) {
    print_indent();
    printf("%s\n", func->Info().c_str());
    this->indent++;
    this->VisitCompundStmt(func->FuncBody);
    this->indent--;
}

void Visitor::VisitFuncProtoType(FuncProtoType* proto) {
}

void Visitor::VisitVarExpr(VarExpr* var) {
    print_indent();
    printf("%s\n", var->Info().c_str());
}

void Visitor::VisitCompundStmt(CompoundStmt* stmt) {
    print_indent();
    printf("%s\n", "CompoundStmt");
    this->indent++;
    for (auto it: stmt->stmts) {
        printf("one stmt[%p]\n", it);
    }
    this->indent--;
}
