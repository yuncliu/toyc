#include "Visitor.h"

Visitor::Visitor() {
}

Visitor::~Visitor() {
}

void Visitor::VisitExpr(Expr* expr) {
}

void Visitor::VisitStmt(Stmt* stmt) {
    printf("Visitor:  %s\n", stmt->Info().c_str());
}

void Visitor::VisitFunc(Func* func) {
    printf("Visitor:  %s\n", func->Info().c_str());
}
