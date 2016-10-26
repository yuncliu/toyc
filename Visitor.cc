#include "Visitor.h"
#include "ASTNode.h"

Visitor::Visitor() {
}

Visitor::~Visitor() {
}

bool Visitor::Visit(std::shared_ptr<ASTNode> s) {
    return true;
}
