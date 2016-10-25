#include "FlexBisonFrontEnd.h"
#include "parser.h"
#include "lex.h"
//extern std::shared_ptr<Stmt> root;

FlexBisonFrontEnd::FlexBisonFrontEnd() {
}

FlexBisonFrontEnd::~FlexBisonFrontEnd() {
}

std::shared_ptr<Stmt> FlexBisonFrontEnd::parse(std::string fileName) {
    yyin = fopen(fileName.c_str(), "r");
    yyparse();
    fclose(yyin);
    std::shared_ptr<Stmt> p;
    return p;
}
