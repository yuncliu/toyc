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
#include "Visitor.h"
#define  LOG(fmt, args...)  if (0) printf(fmt, ##args)
using namespace llvm;

class Expr;
class IdExpr;
class IntExpr;
class VarExpr;
class BinaryExpr;
class Stmt;
class ReturnStmt;
class BlockAST;
class FuncArgsAST;
class FuncProtoType;
class FuncCallArgs;
class Func;
class Visitor;

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
    static void ReSet() {
        m = NULL;
        b = NULL;
    }
    static std::map<std::string, Value*> globalNamedValue;
    static bool IsGlobal(std::string name) {
        return Single::globalNamedValue.find(name) != Single::globalNamedValue.end();
    }
};

bool IsAllocaInst(Value* v);
bool IsGlobalVariable(Value* v);


class Expr {
    public:
        Expr();
        virtual ~Expr();
        virtual Value* codegen(BlockAST* block);
};

class IdExpr:public Expr{
        std::string      name;
    public:
        IdExpr(std::string s);
        virtual ~IdExpr();
        Value* codegen(BlockAST* block);
        std::string getName();
};

class IntExpr: Expr {
        int value;
    public:
        IntExpr(int i);
        ~IntExpr();
        Value* codegen(BlockAST* block);
};

class DoubleExpr: Expr {
        double value;
    public:
        DoubleExpr(double d);
        ~DoubleExpr();
        Value* codegen(BlockAST* block);
};

class VarExpr: Expr {
        Type* type;
        IdExpr* Id;
    public:
        VarExpr(Type* ty, IdExpr* id);
        ~VarExpr();
        Value* codegen(BlockAST* block);
        Type* getType();
        std::string getName();
        std::string Info();
};

class FuncCallExpr: Expr {
    IdExpr* Id;
    FuncCallArgs* Args;
    public:
        FuncCallExpr(IdExpr* id, FuncCallArgs* args);
        ~FuncCallExpr();
        Value* codegen(BlockAST* block);
};

class BinaryExpr {
        char op;
        Expr* left;
        Expr* right;
    public:
        BinaryExpr(char op, Expr* l, Expr* r);
        virtual ~BinaryExpr();
        virtual Value* codegen(BlockAST* block);
};

class Stmt {
    protected:
        Expr* value;
    public:
        Stmt();
        Stmt(Expr* e);
        ~Stmt();
        virtual void codegen(BlockAST* block);
        virtual std::string Info();
        virtual void Accept(Visitor* v);
};

class ReturnStmt: public Stmt {
    public:
        ReturnStmt(Expr* e);
        ~ReturnStmt();
        virtual void codegen(BlockAST* block);
        std::string Info();
};

class VarStmt: public Stmt {
    public:
        VarStmt(Expr* v);
        ~VarStmt();
        virtual void codegen(BlockAST* block);
};

class BlockAST {
        std::vector<Stmt*> stmts;
        std::map<std::string, Value*> locals;
        BlockAST* Parent;
    public:
        BasicBlock* block;
        BlockAST();
        ~BlockAST();
        virtual void codegen();
        void addLocalVariable(std::string n, Value* v);
        Value* getLocalVariable(std::string n);
        void addStatement(Stmt* s);
        void setParent(BlockAST* p);
        void Accept(Visitor* v);
};

class FuncArgsAST {
        std::vector<std::string> Names;
        std::vector<Type*>  Args;
        std::vector<VarExpr*>  ParmVars;
    public:
        FuncArgsAST();
        ~FuncArgsAST();
        void addArg(VarExpr* v);
        std::vector<std::string> getArgNames();
        size_t getArgSize();
        std::vector<Type*> getArgs();
        Type* getArgType(size_t i);
        std::string getArgName(size_t i);
        void Accept(Visitor* v);
};

class FuncProtoType {
        IdExpr* Id;
        Type* ReturnTy;
        FuncArgsAST* Args;
    public:
        FuncProtoType(IdExpr* i, Type* rty, FuncArgsAST* args);
        ~FuncProtoType();
        FunctionType* getFunctionType();
        Function* codegen();
        std::string getName();
        size_t getArgSize();
        Type* getArgType(size_t i);
        std::string getArgName(size_t i);
        std::vector<Type*> getArgs();
        void Accept(Visitor* v);
};

class FuncCallArgs {
    std::vector<Expr*> Args;
    public:
    FuncCallArgs();
    ~FuncCallArgs();
    void pushArg(Expr* arg);
    std::vector<Value*> getArgs(BlockAST* block);
};

class Func: Stmt {
        FuncProtoType* ProtoType;
        BlockAST* FuncBody;
    public:
        Func(FuncProtoType* f, BlockAST* b);
        ~Func();
        virtual void codegen(BlockAST* block);
        std::string getName();
        virtual std::string Info();
        virtual void Accept(Visitor* v);
};

class IfStmt: Stmt {
    Expr* Cond;
    BlockAST* Then;
    BlockAST* Else;
    public:
    IfStmt(Expr* Cond, BlockAST* Then, BlockAST* Else);
    ~IfStmt();
    virtual void codegen(BlockAST* block);
};
#endif
