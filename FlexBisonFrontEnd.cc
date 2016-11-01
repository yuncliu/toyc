#include "FlexBisonFrontEnd.h"
#include "parser.h"
#include "lex.h"
#include <algorithm>
#include <iostream>
//extern std::shared_ptr<Stmt> root;
//std::shared_ptr<ASTNode> ast(NULL);
std::shared_ptr<ASTNode> FlexBisonFrontEnd::ast = std::shared_ptr<ASTNode>(new ASTNode());

FlexBisonFrontEnd::FlexBisonFrontEnd() {
}

FlexBisonFrontEnd::~FlexBisonFrontEnd() {
}

std::shared_ptr<ASTNode> FlexBisonFrontEnd::parse(std::string fileName) {
    yyin = fopen(fileName.c_str(), "r");
    yyparse();
    fclose(yyin);
    ast_beautify(ast);
    return ast;
}

void FlexBisonFrontEnd::ast_beautify(std::shared_ptr<ASTNode>& ast) {
    if (!ast || ast->children.empty()) {
        return;
    }
    if (ast->type == ASTNode::VarDecl) {
        std::shared_ptr<VarDecl> var = ast->get<VarDecl>();
        if (var) {
            ast = std::dynamic_pointer_cast<ASTNode>(var);
        }
    }

    if (ast->type == ASTNode::IntegerLiteral) {
        std::shared_ptr<IntegerLiteral> var = ast->get<IntegerLiteral>();
        if (var) {
            ast = std::dynamic_pointer_cast<ASTNode>(var);
        }
    }

    for (auto& it : ast->children) {
        ast_beautify(it);
    }
}

std::shared_ptr<ASTNode> FlexBisonFrontEnd::node_beautify(std::shared_ptr<ASTNode> ast) {
    if (ast->type == ASTNode::VarDecl) {
        printf("fuck\n");
        std::shared_ptr<VarDecl> var = ast->get<VarDecl>();
        return var;
    }
    for (auto& it : ast->children) {
        it = node_beautify(it);
    }
    return ast;
}
