#include "LLVMVisitor.h"
#include "ast.h"
#include "Expr.h"
#include "Stmt.h"
#include "llvm/IR/ValueSymbolTable.h"

LLVMVisitor::LLVMVisitor() {
    module = new Module("main", getGlobalContext());
    builder = new IRBuilder<>(getGlobalContext());
    CurrentFunction = NULL;
    CurrentBlock = NULL;
}

LLVMVisitor::~LLVMVisitor() {
    this->module->dump();
}

bool LLVMVisitor::Visit(Stmt* s) {
    CodeGenForStmt(s);
    return true;
}

Module* LLVMVisitor::getModule() {
    return module;
}

Value* LLVMVisitor::CodeGenForFunc(Stmt* stmt) {
    Func* f = (Func*)stmt;
    Function* func = CodeGenForFuncProtoType(f->ProtoType);
    CurrentFunction = func;
    BasicBlock* block = BasicBlock::Create(getGlobalContext(), "entry", func);
    CurrentBlock = block;
    builder->SetInsertPoint(block);
    //this->Visit(f->FuncBody);
    unsigned i = 0;
    Function::arg_iterator it;
    for (it = func->arg_begin(); it != func->arg_end(); ++it) {
        Value* v = CodeGenForVarExpr(f->ProtoType->Param->Params[i]);
        builder->CreateStore(&*it, v);
        i++;
    }
    this->CodeGenForCompoundStmt(f->FuncBody);
    CurrentBlock = NULL;
    CurrentFunction = NULL;
    return func;
}

Function* LLVMVisitor::CodeGenForFuncProtoType(Stmt* stmt) {
    FuncProtoType* p = static_cast<FuncProtoType*>(stmt);
    Type* rty = CodeGenForTypeExpr(p->ReturnTy);
    std::vector<Type*> params = CodeGenForFuncParams(p->Param);
    FunctionType* fty = FunctionType::get(rty, params, false);
    Function* fun = Function::Create(fty,
            Function::ExternalLinkage,
            p->Id->Id,
            module);

    for (auto it:p->Param->Params) {
        printf("----------[%s]\n", it->getSelfName().c_str());
    }

    return fun;
}

Type* LLVMVisitor::CodeGenForTypeExpr(Stmt* stmt) {
    TypeExpr* p = static_cast<TypeExpr*>(stmt);
    if (p->Type == "int") {
        return Type::getInt32Ty(getGlobalContext());
    }
    if (p->Type == "double") {
        return Type::getDoubleTy(getGlobalContext());
    }
    return NULL;
}

std::vector<Type*> LLVMVisitor::CodeGenForFuncParams(Stmt* stmt) {
    FuncParameter* p = static_cast<FuncParameter*>(stmt);
    std::vector<Type*> v;
    for (auto it: p->Params) {
        VarExpr* pp = static_cast<VarExpr*>(it);
        v.push_back(CodeGenForTypeExpr(pp->Type));
    }
    return v;
}

Value* LLVMVisitor::CodeGenForCompoundStmt(Stmt* stmt) {
    CompoundStmt* p = (CompoundStmt*)stmt;
    int size = p->stmts.size();
    for (int i = 0; i < size; ++i) {
        this->CodeGenForStmt(p->stmts[i]);
    }
    return NULL;
}

Value* LLVMVisitor::CodeGenForStmt(Stmt* stmt) {
    if (stmt->getSelfName() == "CompoundStmt") {
        return this->CodeGenForCompoundStmt(stmt);
    }
    if (stmt->getSelfName() == "ReturnStmt") {
        return this->CodeGenForReturnStmt(stmt);
    }
    if (stmt->getSelfName() == "BinaryExpr") {
        return this->CodeGenForBinaryExpr(stmt);
    }
    if (stmt->getSelfName() == "IdExpr") {
        return this->CodeGenForIdExpr(stmt);
    }
    if (stmt->getSelfName() == "IntExpr") {
        return this->CodeGenForIntExpr(stmt);
    }
    if (stmt->getSelfName() == "VarExpr") {
        return this->CodeGenForVarExpr(stmt);
    }
    if (stmt->getSelfName() == "Func") {
        return this->CodeGenForFunc(stmt);
    }
    if (stmt->getSelfName() == "FuncCallExpr") {
        return this->CodeGenForFuncCallExpr(stmt);
    }
    return NULL;
}

Value* LLVMVisitor::CodeGenForReturnStmt(Stmt* stmt) {
    ReturnStmt* p = static_cast<ReturnStmt*>(stmt);
    Value* retval = GetRightValue(CodeGenForStmt(p->Ret));
    return this->builder->CreateRet(retval);
}

Value* LLVMVisitor::CodeGenForBinaryExpr(Stmt* stmt) {
    BinaryExpr* p = static_cast<BinaryExpr*>(stmt);
    Value* l = this->CodeGenForStmt(p->left);
    Value* r = this->CodeGenForStmt(p->right);
    switch(p->op) {
        case '+':
            l = GetRightValue(l);
            r = GetRightValue(r);
            return this->builder->CreateAdd(l,r);
            break;
        case '-':
            l = GetRightValue(l);
            r = GetRightValue(r);
            return this->builder->CreateSub(l,r);
            break;
        case '=':
            if (NULL == CurrentFunction) {
                if (p->left->getSelfName() == "VarExpr") {
                    static_cast<GlobalVariable*>(l)->setInitializer(static_cast<Constant*>(r));
                    return NULL;
                }
            }
            l = GetLeftValue(l);
            r = GetRightValue(r);
            return this->builder->CreateStore(r,l);
            break;
        default:
            break;
    }
    return this->builder->CreateAdd(l,r);
}

Value* LLVMVisitor::CodeGenForIdExpr(Stmt* stmt) {
    IdExpr* p = static_cast<IdExpr*>(stmt);
    if (NULL == CurrentFunction) {
        // if is not in function, must be a global value
        return module->getNamedGlobal(p->Id);
    }
    ValueSymbolTable& vt =  CurrentFunction->getValueSymbolTable();
    Value* v = vt.lookup(p->Id);
    if (NULL == v) {
        // if can't find in function's value table, then lookup in global variable table
        GlobalVariable* g = module->getNamedGlobal(p->Id);
        if (NULL != g) {
            // find in global value table
            return g;
        }
        printf("can't find identifier[%s]\n", p->Id.c_str());
        return ConstantInt::get(Type::getInt32Ty(getGlobalContext()), 0, true);
    }
    return v;
}

Value* LLVMVisitor::CodeGenForIntExpr(Stmt* stmt) {
    IntExpr* p = static_cast<IntExpr*>(stmt);
    return ConstantInt::get(Type::getInt32Ty(getGlobalContext()), p->value, true);
}

Value* LLVMVisitor::CodeGenForVarExpr(Stmt* stmt) {
    VarExpr* p = (VarExpr*)stmt;
    if (NULL == CurrentFunction){
        Type* ty = CodeGenForTypeExpr(p->Type);
        GlobalVariable* global = new GlobalVariable(*module, ty,
                false, Function::InternalLinkage, NULL);
        global->setName(p->Id->Id);
        Constant* initer = ConstantInt::get(ty, 0, true);
        global->setInitializer(initer);
        return global;
    }
    else {
        AllocaInst* Var = builder->CreateAlloca(CodeGenForTypeExpr(p->Type));
        Var->setName(p->Id->Id);
        return Var;
    }
    return NULL;
}

Value* LLVMVisitor::GetLeftValue(Value* v) {
    return v;
}

Value* LLVMVisitor::GetRightValue(Value* v) {
    if (isa<AllocaInst>(v)) {
        return builder->CreateLoad(v);
    }
    if (isa<GlobalVariable>(v)) {
        return builder->CreateLoad(v);
    }
    return v;
}

Value* LLVMVisitor::CodeGenForFuncCallExpr(Stmt* stmt) {
    FuncCallExpr* p = static_cast<FuncCallExpr*>(stmt);
    Function* func = module->getFunction(p->Id->Id);
    std::vector<Value*> args;
    for(auto it : p->Args->Parameters) {
        args.push_back(CodeGenForStmt(it));
    }
    CallInst *call = CallInst::Create(func, makeArrayRef(args), "", CurrentBlock);
    return call;
}
