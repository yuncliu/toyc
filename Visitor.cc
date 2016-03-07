#include "Visitor.h"
#include "Expr.h"
#include "Stmt.h"

Visitor::Visitor() {
}

Visitor::~Visitor() {
}

bool Visitor::Visit(Stmt* stmt) {
    return true;
}
