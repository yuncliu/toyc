#include "ASTNode.h"

ASTNode::ASTNode() {
}

ASTNode::ASTNode(NodeType ty)
    :type(ty) {
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
    :ASTNode(ASTNode::VarDecl) {
}
VarDecl::~VarDecl() {
}
/*
std::string VarDecl::info() {
}
*/
#endif
