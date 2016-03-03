#include "llvm/ADT/STLExtras.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include <stdio.h>
#include "ast.h"
#include "Expr.h"
#include "Stmt.h"
#include "lex.h"
#include "parser.h"
Module* Single::m = NULL;
IRBuilder<>* Single::b = NULL;
std::map<std::string, Value*> Single::globalNamedValue;

int main(int argc, char const* argv[]) {
    Single::getModule();
    Single::getBuilder();
    yyparse();
    Single::getModule()->dump();
    ExecutionEngine* EE = EngineBuilder(std::unique_ptr<Module>(Single::getModule())).create();
    Function* Func = Single::getModule()->getFunction("main");
    std::vector<GenericValue> noargs;
    GenericValue gv = EE->runFunction(Func, noargs);
    printf("Return value is [%ld]\n", *(gv.IntVal.getRawData()));
    delete EE;
    return 0;
}
