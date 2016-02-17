/**
 *
 */
#ifndef _AST_H_
#define _AST_H_
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include <string>
#include <vector>
#define  LOG(fmt, args...)  if (0) printf(fmt, ##args)
using namespace llvm;

class ExprAST;
class IdExprAST;
class IntExprAST;
class VarExprAST;
class BinaryExprAST;
class StmtAST;
class ReturnStmtAST;
class BlockAST;
class FuncArgsAST;
class FuncTypeAST;
class FuncAST;

class Single {
    static Module*      m;
    static IRBuilder<>* b;
    private:
    Single() {
    }
    public:
    static Module* getModule() {
        if (m == NULL) {
            m = new Module("main", getGlobalContext());
        }
        return m;
    }
    static IRBuilder<>* getBuilder() {
        if (b == NULL) {
            b = new IRBuilder<>(getGlobalContext());
        }
        return b;
    }
};


class ExprAST {
    public:
        ExprAST();
        virtual ~ExprAST();
        virtual Value* codegen(BlockAST* block);
        virtual Value* codegen();
};

class IdExprAST:public ExprAST{
    public:
        std::string      name;
        std::string      type;
        IdExprAST(std::string s);
        virtual ~IdExprAST();
        Value* codegen(BlockAST* block);
};

class IntExprAST: ExprAST {
    public:
        int value;
        IntExprAST(int i);
        ~IntExprAST();
        Value* codegen(BlockAST* block);
};

class VarExprAST: ExprAST {
    public:
        std::string type;
        std::string name;
        VarExprAST(std::string t, std::string n);
        ~VarExprAST();
        Value* codegen(BlockAST* block);
};


class BinaryExprAST {
    public:
        char op;
        ExprAST* left;
        ExprAST* right;
        BinaryExprAST(ExprAST* l, ExprAST* r);
        virtual ~BinaryExprAST();
        virtual Value* codegen(BlockAST* block);
};


class StmtAST {
    public:
        ExprAST* value;
        StmtAST();
        StmtAST(ExprAST* e);
        ~StmtAST();
        virtual void codegen(BlockAST* block);
};

class ReturnStmtAST: public StmtAST {
    public:
        ExprAST* expr;
        ReturnStmtAST(ExprAST* e);
        ~ReturnStmtAST();
        virtual void codegen(BlockAST* block);
};

class BlockAST {
    public:
        std::vector<StmtAST*> stmts;
        std::map<std::string, Value*> locals;
        BlockAST();
        ~BlockAST();
        virtual void codegen();
};

class FuncArgsAST {
    public:
        std::vector<std::string> names;
        std::vector<Type*>  args;
        FuncArgsAST();
        ~FuncArgsAST();
        void addarg(std::string name, std::string type);
        std::vector<std::string> getNames();
        std::vector<Type*> getArgs();
};

class FuncTypeAST {
    public:
        FuncArgsAST* arg_list;
        FuncTypeAST();
        ~FuncTypeAST();
        FunctionType* codegen();
};

class FuncAST {
    public:
        std::string name;
        FuncTypeAST* functype;
        BlockAST* body;
        FuncAST(std::string n);
        ~FuncAST();
        Function* codegen();
};

class ProgramAST {
    public:
};

#endif
