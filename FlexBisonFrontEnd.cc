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
    return ast;
}
