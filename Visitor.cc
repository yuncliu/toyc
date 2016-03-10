#include "Visitor.h"
#include "Expr.h"
#include "Stmt.h"

Visitor::Visitor() {
}

Visitor::~Visitor() {
}

bool Visitor::Visit(std::shared_ptr<Stmt> s) {
    return true;
}
