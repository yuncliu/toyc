#ifndef _LLVM_VISITOR_H_
#define _LLVM_VISITOR_H_
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include <map>
#include <vector>
#include "Visitor.h"
#define LIGHT_GREEN "\x1B[0;1;32m"
#define COLOR_BLUE "\x1B[34m"
#define COLOR_RESET "\033[0m"
class LLVMVisitor;
class Stmt;
using namespace llvm;

class LLVMVisitor:public Visitor {
    Module* module;
    IRBuilder<>* builder;
    std::map<std::string, Value*> GlobalNamedValue;
    Function* CurrentFunction;
    BasicBlock* CurrentBlock;
    public:
    LLVMVisitor();
    virtual ~LLVMVisitor();
    virtual bool Visit(Stmt*);
    Module* getModule();
    Value* CodeGenForStmt(Stmt* stmt);
    Value* CodeGenForCompoundStmt(Stmt* stmt);
    Value* CodeGenForFunc(Stmt* stmt);
    Function* CodeGenForFuncProtoType(Stmt* stmt);
    Type* CodeGenForTypeExpr(Stmt* stmt);
    std::vector<Type*> CodeGenForFuncParams(Stmt* stmt);
    Value* CodeGenForBinaryExpr(Stmt* stmt);
    Value* CodeGenForReturnStmt(Stmt* stmt);
    Value* CodeGenForIdExpr(Stmt* stmt);
    Value* CodeGenForIntExpr(Stmt* stmt);
    Value* CodeGenForVarExpr(Stmt* stmt);
    Value* GetLeftValue(Value* v);
    Value* GetRightValue(Value* v);
    Value* CodeGenForFuncCallExpr(Stmt* stmt);
};

#endif // _VISITOR_H_
