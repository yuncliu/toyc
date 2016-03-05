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
    typedef bool (LLVMVisitor::*VISIT_FUNC)(Stmt* s);
    std::map<std::string, VISIT_FUNC> functions;
    std::map<std::string, Value*> NamedValue;
    Module* module;
    IRBuilder<>* builder;
    std::map<std::string, Value*> GlobalNamedValue;
    public:
    LLVMVisitor();
    virtual ~LLVMVisitor();
    VISIT_FUNC getFunction(std::string name);
    virtual bool Visit(Stmt*);
    Value* CodeGenForStmt(Stmt* stmt);
    bool VisitCompoundStmt(Stmt* stmt);
    Value* CodeGenForCompoundStmt(Stmt* stmt);
    bool VisitFunc(Stmt* stmt);
    Function* CodeGenForFuncProtoType(Stmt* stmt);
    Type* CodeGenForTypeExpr(Stmt* stmt);
    bool VisitFuncProtoType(Stmt* stmt);
    bool VisitIdExpr(Stmt* stmt);
    bool VisitTypeExpr(Stmt* stmt);
    bool VisitFuncParameter(Stmt* stmt);
    std::vector<Type*> CodeGenForFuncParams(Stmt* stmt);
    //std::vector<Type*> GetTypeVecForFuncParams(Stmt* stmt);
    //std::vector<string> GetNameVecForFuncParams(Stmt* stmt);
    bool VisitVarExpr(Stmt* stmt);
    bool VisitBinaryExpr(Stmt* stmt);
    Value* CodeGenForBinaryExpr(Stmt* stmt);
    bool VisitReturnStmt(Stmt* stmt);
    Value* CodeGenForReturnStmt(Stmt* stmt);
    bool VisitIntExpr(Stmt* stmt);
    Value* CodeGenForIdExpr(Stmt* stmt);
    Value* CodeGenForIntExpr(Stmt* stmt);
    Value* CodeGenForVarExpr(Stmt* stmt);
};

#endif // _VISITOR_H_
