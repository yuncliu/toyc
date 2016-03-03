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
}

void Visitor::VisitFuncProtoType(FuncProtoType* proto) {
}

void Visitor::VisitVarExpr(VarExpr* var) {
    print_indent();
    printf("%s\n", var->Info().c_str());
}
