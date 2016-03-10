#ifndef _LLVM_VISITOR_H_
#define _LLVM_VISITOR_H_
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Transforms/Utils/Cloning.h"
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
    std::shared_ptr<Module> module;
    std::shared_ptr<IRBuilder<> > builder;
    std::map<std::string, Value*> GlobalNamedValue;
    Function* CurrentFunction;
    BasicBlock* CurrentBlock;
    public:
    LLVMVisitor();
    virtual ~LLVMVisitor();
    virtual bool Visit(std::shared_ptr<Stmt>);
    std::unique_ptr<Module> getModule();
    Value* CodeGenForStmt(std::shared_ptr<Stmt> stmt);
    Value* CodeGenForCompoundStmt(std::shared_ptr<Stmt> stmt);
    Value* CodeGenForFunc(std::shared_ptr<Stmt> stmt);
    Function* CodeGenForFuncProtoType(std::shared_ptr<Stmt> stmt);
    Type* CodeGenForTypeExpr(std::shared_ptr<Stmt> stmt);
    std::vector<Type*> CodeGenForFuncParams(std::shared_ptr<Stmt> stmt);
    Value* CodeGenForBinaryExpr(std::shared_ptr<Stmt> stmt);
    Value* CodeGenForReturnStmt(std::shared_ptr<Stmt> stmt);
    Value* CodeGenForIdExpr(std::shared_ptr<Stmt> stmt);
    Value* CodeGenForIntExpr(std::shared_ptr<Stmt> stmt);
    Value* CodeGenForVarExpr(std::shared_ptr<Stmt> stmt);
    Value* CodeGenForFuncCallExpr(std::shared_ptr<Stmt> stmt);
    Value* GetLeftValue(Value* v);
    Value* GetRightValue(Value* v);
};

#endif // _VISITOR_H_
