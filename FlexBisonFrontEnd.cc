#include "FlexBisonFrontEnd.h"
#include "parser.h"
#include "lex.h"
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
//    std::shared_ptr<ASTNode> p;
    ast_beautify(ast);
    return ast;
}

void FlexBisonFrontEnd::ast_beautify(std::shared_ptr<ASTNode>& ast) {
    if (ast->children.empty()) {
        return;
    }
    for (auto& it : ast->children) {
        it = node_beautify(it);
    }
}

std::shared_ptr<ASTNode> FlexBisonFrontEnd::node_beautify(std::shared_ptr<ASTNode> ast) {
    if (ast->type == ASTNode::VarDecl) {
        std::shared_ptr<VarDecl> var(new VarDecl);
        var->type_specifier = "hellow";
        for_each(ast->children.begin(), ast->children.end(),
                [&](std::shared_ptr<ASTNode> it) {
            if (it->type == ASTNode::TYPE_SPECIFIER) {
                var->type_specifier = it->value;
            }
            /*
            if (it->type == ASTNode::INIT_DECLARATOR) {
                printf("fuck\n");
                for_each(it->children.begin(),
                        it->children.end(),
                        [&](std::shared_ptr<ASTNode> itt){
                    if (itt->type == ASTNode::IDENTIFIER) {
                        var->identifier = itt->value;
                        printf("fuck\n");
                    }
                    if (itt->type == ASTNode::CONSTANT) {
                        var->children.push_back(itt->children[0]);
                    }
                });
            }
            */
        });
        return var;
    }
    return ast;
}
