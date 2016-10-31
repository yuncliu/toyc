#include "ASTNode.h"

ASTNode::ASTNode() {
}

ASTNode::ASTNode(std::string v, NodeType ty)
    :value(v), type(ty) {
}

ASTNode::~ASTNode() {
}

std::string ASTNode::info() {
    return this->value;
}

#if 1
/*
 * @class VarDecl
 */
VarDecl::VarDecl()
    :ASTNode("VarDecl", ASTNode::VarDecl) {
}
VarDecl::~VarDecl() {
}

std::string VarDecl::info() {
    return this->value + " " +this->type_specifier;
}

#endif
