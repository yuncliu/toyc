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
class FuncCallArgs;
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
    static bool IsGlobal(std::string name) {
        return Single::globalNamedValue.find(name) != Single::globalNamedValue.end();
    }
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
        int value;
    public:
        IntExprAST(int i);
        ~IntExprAST();
        Value* codegen(BlockAST* block);
};

class VarExprAST: ExprAST {
        Type* type;
        IdExprAST* Id;
    public:
        VarExprAST(Type* ty, IdExprAST* id);
        ~VarExprAST();
        Value* codegen(BlockAST* block);
        std::string getName();
        Type* getType();
};

class FuncCallExpr: ExprAST {
    IdExprAST* Id;
    FuncCallArgs* Args;
    public:
        FuncCallExpr(IdExprAST* id, FuncCallArgs* args);
        ~FuncCallExpr();
        Value* codegen(BlockAST* block);
};

class BinaryExprAST {
        char op;
        ExprAST* left;
        ExprAST* right;
    public:
        BinaryExprAST(char op, ExprAST* l, ExprAST* r);
        virtual ~BinaryExprAST();
        virtual Value* codegen(BlockAST* block);
};

class StmtAST {
        ExprAST* value;
    public:
        StmtAST();
        StmtAST(ExprAST* e);
        ~StmtAST();
        virtual void codegen(BlockAST* block);
};

class ReturnStmtAST: public StmtAST {
        ExprAST* expr;
    public:
        ReturnStmtAST(ExprAST* e);
        ~ReturnStmtAST();
        virtual void codegen(BlockAST* block);
};

class VarStmtAST: public StmtAST {
        VarExprAST* value;
    public:
        VarStmtAST(VarExprAST* v);
        ~VarStmtAST();
        virtual void codegen(BlockAST* block);
};

class BlockAST {
        std::vector<StmtAST*> stmts;
        std::map<std::string, Value*> locals;
        BlockAST* Parent;
    public:
        BasicBlock* block;
        BlockAST();
        ~BlockAST();
        virtual void codegen();
        void addLocalVariable(std::string n, Value* v);
        Value* getLocalVariable(std::string n);
        void addStatement(StmtAST* s);
        void setParent(BlockAST* p);
};

class FuncArgsAST {
        std::vector<std::string> Names;
        std::vector<Type*>  Args;
    public:
        FuncArgsAST();
        ~FuncArgsAST();
        void addArg(VarExprAST* v);
        std::vector<std::string> getArgNames();
        size_t getArgSize();
        std::vector<Type*> getArgs();
        Type* getArgType(size_t i);
        std::string getArgName(size_t i);
};

class FuncProtoType {
        IdExprAST* Id;
        Type* ReturnTy;
        FuncArgsAST* Args;
    public:
        FuncProtoType(IdExprAST* i, Type* rty, FuncArgsAST* args);
        ~FuncProtoType();
        FunctionType* getFunctionType();
        Function* codegen();
        std::string getName();
        size_t getArgSize();
        Type* getArgType(size_t i);
        std::string getArgName(size_t i);
        std::vector<Type*> getArgs();
};

class FuncCallArgs {
    std::vector<ExprAST*> Args;
    public:
    FuncCallArgs();
    ~FuncCallArgs();
    void pushArg(ExprAST* arg);
    std::vector<Value*> getArgs(BlockAST* block);
};

class FuncAST: StmtAST {
        FuncProtoType* ProtoType;
        BlockAST* FuncBody;
    public:
        FuncAST(FuncProtoType* f, BlockAST* b);
        ~FuncAST();
        virtual void codegen(BlockAST* block);
        std::string getName();
};

class IfStmtAST: StmtAST {
    ExprAST* Cond;
    BlockAST* Body;
    public:
    IfStmtAST(ExprAST* Cond, BlockAST* block);
    ~IfStmtAST();
    virtual void codegen(BlockAST* block);
};

class ProgramAST {
    public:
};

#endif
