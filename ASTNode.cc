#include "ASTNode.h"
#include <algorithm>
#include <iostream>
#include <sstream>

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

void ASTNode::initialize(ASTNode* _node) {
    this->value = _node->value;
    this->type = _node->type;
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
    return this->value + " type = " +this->type_specifier + "[" + this->identifier + "]";
}

void VarDecl::initialize(ASTNode* _node) {
    //return;
    for_each(_node->children.begin(), _node->children.end(),
            [&](std::shared_ptr<ASTNode> it) {
        if (it->type == ASTNode::TYPE_SPECIFIER) {
            this->type_specifier = it->value;
        }
        if (it->type == ASTNode::INIT_DECLARATOR) {
            /* The init_declarator has 2 children  [0] is identifier and [1] is the value*/
            this->identifier = it->children[0]->value;
            this->children.push_back(it->children[1]);
        }
    });
}

#endif

IntegerLiteral::IntegerLiteral()
    :ASTNode("IntegerLiteral", ASTNode::IntegerLiteral){
}

IntegerLiteral::~IntegerLiteral() {
}

std::string IntegerLiteral::info(){
    std::stringstream ssm;
    ssm<<  "IntegerLiteral [" << this->value << "]";
    return ssm.str();//"IntegerLiteral" + "[" + this->value  + "]";
}

void IntegerLiteral::initialize(ASTNode* _node) {
    this->value = _node->value;
}

FloatingLiteral::FloatingLiteral()
    :ASTNode("", ASTNode::FloatingLiteral) {
}

FloatingLiteral::~FloatingLiteral() {
}

std::string FloatingLiteral::info() {
    std::stringstream ssm;
    ssm << "FloatingLiteral [" << this->value << "]";
    return ssm.str();//"IntegerLiteral" + "[" + this->value  + "]";
}

/**
* @brief initilaize from orginal ASTNode
*/
void FloatingLiteral::initialize(ASTNode* _node) {
}


BinaryOperator::BinaryOperator()
    :ASTNode("", ASTNode::BinaryOperator) {
}

BinaryOperator::~BinaryOperator() {
}

std::string BinaryOperator::info() {
    std::stringstream ssm;
    ssm << "BinaryOperator [" << this->op << "]";
    return ssm.str();//"IntegerLiteral" + "[" + this->value  + "]";
}

/**
 * @brief initilaize from orginal ASTNode
 */
void BinaryOperator::initialize(ASTNode* _node) {
}

Identifier::Identifier()
    :ASTNode("", IDENTIFIER) {
}

Identifier::~Identifier() {
}

std::string Identifier::info() {
    std::stringstream ssm;
    ssm << "Identifier [" << this->value << "]";
    return ssm.str();//"IntegerLiteral" + "[" + this->value  + "]";
}

void Identifier::initialize(ASTNode* _node) {
}
