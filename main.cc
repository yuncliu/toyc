#include <stdio.h>
#include "ast.h"
#include "lex.h"
#include "parser.h"
Module* Single::m = NULL;
IRBuilder<>* Single::b = NULL;

int main(int argc, char const* argv[]) {
    Single::getModule();
    Single::getBuilder();
    yyparse();
    Single::getModule()->dump();
    return 0;
}
