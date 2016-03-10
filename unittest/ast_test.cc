#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "gtest/gtest.h"
#include "Stmt.h"
#include "Expr.h"
#include "LLVMVisitor.h"
#include "lex.h"
#include "parser.h"
//Stmt* root = NULL;
std::shared_ptr<Stmt> root(NULL);
class ASTTest: public testing::Test {
    protected:
        ExecutionEngine* EE;
        LLVMVisitor* visitor;
        virtual void SetUp() {
            visitor = new LLVMVisitor();
            EE = NULL;
        }

        int exe() {
            printf("start to exec\n");
            EE = EngineBuilder(std::unique_ptr<Module>(visitor->getModule())).create();
            Function* Func = visitor->getModule()->getFunction("main");
            std::vector<GenericValue> noargs;
            GenericValue gv = EE->runFunction(Func, noargs);
            printf("Return value is [%ld]\n", *(gv.IntVal.getRawData()));
            return *(gv.IntVal.getRawData());
            return 0;
        }

        virtual void TearDown() {
            if (NULL != EE) {
                delete EE;
                EE = NULL;
            }
            if (NULL != visitor) {
                delete visitor;
                visitor = NULL;
            }
        }
};

TEST_F(ASTTest, return0) {

    yy_scan_string("\
            int main() {\
            return 10;\
            }");
    yyparse();
    visitor->Visit(root);
    EXPECT_EQ(this->exe(), 10);
}

TEST_F(ASTTest, return_variable) {
    yy_scan_string("int main() {\
            int a = 10;\
            return a;\
            }");
    yyparse();
    visitor->Visit(root);
    EXPECT_EQ(this->exe(), 10);
}

TEST_F(ASTTest, return_global_variable) {
    yy_scan_string("int a = 10;\
            int main() {\
            return a;\
            }");
    yyparse();
    visitor->Visit(root);
    visitor->getModule()->dump();
    EXPECT_EQ(this->exe(), 10);
}

TEST_F(ASTTest, return_function) {
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
    visitor->Visit(root);
    visitor->getModule()->dump();
    EXPECT_EQ(this->exe(), 11);
}

TEST_F(ASTTest, if_else) {
    yy_scan_string("\
            int main() {\
            if (0) {\
            return 0;\
            }else {\
            return 1;\
            }\
            }");
    yyparse();
    visitor->Visit(root);
    EXPECT_EQ(this->exe(), 1);
}

TEST_F(ASTTest, double2int) {
    yy_scan_string("\
            int main() {\
                double a = 10.8;\
                int b = a;\
                return b;\
            }");
    yyparse();
    visitor->Visit(root);
    EXPECT_EQ(this->exe(), 10);
}

TEST_F(ASTTest, int2double) {
    yy_scan_string("\
            int main() {\
                int a = 10;\
                double b = a;\
                int c = a;\
                return c;\
            }");
    yyparse();
    visitor->Visit(root);
    EXPECT_EQ(this->exe(), 10);
}
