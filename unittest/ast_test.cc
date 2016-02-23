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
    Single::ReSet();
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
}

TEST(main, return_variable) {
    Single::ReSet();
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

TEST(main, return_global_variable) {
    Single::ReSet();
    Single::getModule();
    Single::getBuilder();

    yy_scan_string("int a = 10;"
                   "int main() {"
                   "    return a;"
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

TEST(main, return_function) {
    Single::ReSet();
    Single::getModule();
    Single::getBuilder();

    yy_scan_string("\
            int foo (int a) {\
                a = 10;\
                return a+1;\
            }\
            int main()\
            {\
                return foo(1);\
            }");

    yyparse();
    Single::getModule()->dump();
    ExecutionEngine* EE = EngineBuilder(std::unique_ptr<Module>(Single::getModule())).create();
    Function* Func = Single::getModule()->getFunction("main");
    std::vector<GenericValue> noargs;
    GenericValue gv = EE->runFunction(Func, noargs);
    printf("Return value is [%ld]\n", *(gv.IntVal.getRawData()));
    EXPECT_EQ(*(gv.IntVal.getRawData()), 11);
    delete EE;
    Single::ReSet();
}

TEST(main, if_else) {
    Single::ReSet();
    Single::getModule();
    Single::getBuilder();

    yy_scan_string("\
int main() {\
    if (0) {\
        return 0;\
    }else {\
        return 1;\
    }\
}");

    yyparse();
    Single::getModule()->dump();
    ExecutionEngine* EE = EngineBuilder(std::unique_ptr<Module>(Single::getModule())).create();
    Function* Func = Single::getModule()->getFunction("main");
    std::vector<GenericValue> noargs;
    GenericValue gv = EE->runFunction(Func, noargs);
    printf("Return value is [%ld]\n", *(gv.IntVal.getRawData()));
    EXPECT_EQ(*(gv.IntVal.getRawData()), 1);
    delete EE;
    Single::ReSet();
}
