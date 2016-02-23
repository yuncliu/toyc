#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "gtest/gtest.h"
#include "ast.h"
#include "lex.h"
#include "parser.h"
Module* Single::m = NULL;
IRBuilder<>* Single::b = NULL;
std::map<std::string, Value*> Single::globalNamedValue;

TEST(main, return0) {
    Single::getModule();
    Single::getBuilder();

    yy_scan_string("int main() {"
                       "return 10;"
                   "}");
    yyparse();
    Single::getModule()->dump();
    ExecutionEngine* EE = EngineBuilder(std::unique_ptr<Module>(Single::getModule())).create();
    Function* Func = Single::getModule()->getFunction("main");
    std::vector<GenericValue> noargs;
    GenericValue gv = EE->runFunction(Func, noargs);
    printf("Return value is [%ld]\n", *(gv.IntVal.getRawData()));
    EXPECT_EQ(*(gv.IntVal.getRawData()), 10);
    delete EE;
    Single::ReSet();
}

TEST(main, return_variable) {
    Single::getModule();
    Single::getBuilder();

    yy_scan_string("int main() {"
                       "int a = 10;"
                       "return a;"
                   "}");
    yyparse();
    Single::getModule()->dump();
    ExecutionEngine* EE = EngineBuilder(std::unique_ptr<Module>(Single::getModule())).create();
    Function* Func = Single::getModule()->getFunction("main");
    std::vector<GenericValue> noargs;
    GenericValue gv = EE->runFunction(Func, noargs);
    printf("Return value is [%ld]\n", *(gv.IntVal.getRawData()));
    EXPECT_EQ(*(gv.IntVal.getRawData()), 10);
    delete EE;
}
