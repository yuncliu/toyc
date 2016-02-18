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
class FuncProtoType;
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
    static std::map<std::string, Value*> globalNamedValue;
};


class ExprAST {
    public:
        ExprAST();
        virtual ~ExprAST();
        virtual Value* codegen(BlockAST* block);
};

class IdExprAST:public ExprAST{
        std::string      name;
    public:
        IdExprAST(std::string s);
        virtual ~IdExprAST();
        Value* codegen(BlockAST* block);
        std::string getName();
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
        Type* type;
        std::string name;
        VarExprAST(Type* ty, std::string n);
        ~VarExprAST();
        Value* codegen(BlockAST* block);
};


class BinaryExprAST {
    public:
        char op;
        ExprAST* left;
        ExprAST* right;
        BinaryExprAST(char op, ExprAST* l, ExprAST* r);
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

class VarStmtAST: public StmtAST {
    public:
        ExprAST* var;
        VarStmtAST(ExprAST* e);
        ~VarStmtAST();
        virtual void codegen(BlockAST* block);
};

class BlockAST {
    public:
        std::vector<StmtAST*> stmts;
        std::map<std::string, Value*> locals;
        BasicBlock* block;
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
        void addarg(std::string name, Type* type);
        std::vector<std::string> getNames();
        std::vector<Type*> getArgs();
};

class FuncProtoType {
    public:
        IdExprAST* Id;
        Type* ReturnTy;
        FuncArgsAST* Args;
        FuncProtoType(IdExprAST* i, Type* rty, FuncArgsAST* args);
        ~FuncProtoType();
        FunctionType* codegen();
        std::string getName();
};

class FuncAST: StmtAST {
    public:
        FuncProtoType* ProtoType;
        BlockAST* FuncBody;
        FuncAST(FuncProtoType* f, BlockAST* b);
        ~FuncAST();
        virtual void codegen(BlockAST* block);
        std::string getName();
};

class ProgramAST {
    public:
};

#endif
