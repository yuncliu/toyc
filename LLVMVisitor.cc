#include "LLVMVisitor.h"
#include "Expr.h"
#include "Stmt.h"
#include "llvm/IR/ValueSymbolTable.h"

LLVMVisitor::LLVMVisitor()
    :module(new Module("main", getGlobalContext())),
    builder(new IRBuilder<>(getGlobalContext())) {
    CurrentFunction = NULL;
    CurrentBlock = NULL;
}

LLVMVisitor::~LLVMVisitor() {
    printf("LLVM IR:\n");
    this->module->dump();
}

bool LLVMVisitor::Visit(std::shared_ptr<Stmt> s) {
    CodeGenForStmt(s);
    return true;
}

std::unique_ptr<Module> LLVMVisitor::getModule() {
    return llvm::CloneModule(module.get());
}

Value* LLVMVisitor::CodeGenForFunc(std::shared_ptr<Stmt> stmt) {
    //Func* f = static_cast<Func*>(stmt);
    std::shared_ptr<Func> p = std::static_pointer_cast<Func>(stmt);
    Function* func = CodeGenForFuncProtoType(p->ProtoType);
    CurrentFunction = func;
    BasicBlock* block = BasicBlock::Create(getGlobalContext(), "entry", func);
    CurrentBlock = block;
    builder->SetInsertPoint(block);
    // static to generate function body
    unsigned i = 0;
    Function::arg_iterator it;
    for (it = func->arg_begin(); it != func->arg_end(); ++it) {
        Value* v = CodeGenForVarExpr(p->ProtoType->Param->Params[i]);
        builder->CreateStore(&*it, v);
        i++;
    }
    this->CodeGenForCompoundStmt(p->FuncBody);
    CurrentBlock = NULL;
    CurrentFunction = NULL;
    return func;
}

Function* LLVMVisitor::CodeGenForFuncProtoType(std::shared_ptr<Stmt> stmt) {
    //FuncProtoType* p = static_cast<FuncProtoType*>(stmt);
    std::shared_ptr<FuncProtoType> p = std::static_pointer_cast<FuncProtoType>(stmt);
    Type* rty = CodeGenForTypeExpr(p->ReturnTy);
    std::vector<Type*> params = CodeGenForFuncParams(p->Param);
    FunctionType* fty = FunctionType::get(rty, params, false);
    Function* fun = Function::Create(fty,
            Function::ExternalLinkage,
            p->Id->Id,
            module.get());
    return fun;
}

Type* LLVMVisitor::CodeGenForTypeExpr(std::shared_ptr<Stmt> stmt) {
    //TypeExpr* p = static_cast<TypeExpr*>(stmt);
    std::shared_ptr<TypeExpr> p = std::static_pointer_cast<TypeExpr>(stmt);
    if (p->Type == "int") {
        return Type::getInt32Ty(getGlobalContext());
    }
    if (p->Type == "double") {
        return Type::getDoubleTy(getGlobalContext());
    }
    return NULL;
}

std::vector<Type*> LLVMVisitor::CodeGenForFuncParams(std::shared_ptr<Stmt> stmt) {
    std::shared_ptr<FuncParameter> p = std::static_pointer_cast<FuncParameter>(stmt);
    std::vector<Type*> v;
    for (auto it: p->Params) {
        std::shared_ptr<VarExpr> p = std::static_pointer_cast<VarExpr>(it);
        v.push_back(CodeGenForTypeExpr(p->Type));
    }
    return v;
}

Value* LLVMVisitor::CodeGenForCompoundStmt(std::shared_ptr<Stmt> stmt) {
    std::shared_ptr<CompoundStmt> p = std::static_pointer_cast<CompoundStmt>(stmt);
    for (auto it : p->stmts) {
        this->CodeGenForStmt(it);
    }
    return NULL;
}

Value* LLVMVisitor::CodeGenForStmt(std::shared_ptr<Stmt> stmt) {
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
    if (stmt->getSelfName() == "IfStmt") {
        return this->CodeGenForIfStmt(stmt);
    }
    if (stmt->getSelfName() == "AssignStmt") {
        return this->CodeGenForAssignStmt(stmt);
    }
    return NULL;
}

Value* LLVMVisitor::CodeGenForReturnStmt(std::shared_ptr<Stmt> stmt) {
    std::shared_ptr<ReturnStmt> p = std::static_pointer_cast<ReturnStmt>(stmt);
    Value* retval = GetRightValue(CodeGenForStmt(p->Ret));
    return this->builder->CreateRet(retval);
}

Value* LLVMVisitor::CodeGenForBinaryExpr(std::shared_ptr<Stmt> stmt) {
    std::shared_ptr<BinaryExpr> p = std::static_pointer_cast<BinaryExpr>(stmt);
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
        default:
            break;
    }
    return this->builder->CreateAdd(l,r);
}

Value* LLVMVisitor::CodeGenForIdExpr(std::shared_ptr<Stmt> stmt) {
    std::shared_ptr<IdExpr> p = std::static_pointer_cast<IdExpr>(stmt);
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

Value* LLVMVisitor::CodeGenForIntExpr(std::shared_ptr<Stmt> stmt) {
    std::shared_ptr<IntExpr> p = std::static_pointer_cast<IntExpr>(stmt);
    return ConstantInt::get(Type::getInt32Ty(getGlobalContext()), p->value, true);
}

Value* LLVMVisitor::CodeGenForVarExpr(std::shared_ptr<Stmt> stmt) {
    //VarExpr* p = (VarExpr*)stmt;
    std::shared_ptr<VarExpr> p = std::static_pointer_cast<VarExpr>(stmt);
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

Value* LLVMVisitor::CodeGenForFuncCallExpr(std::shared_ptr<Stmt> stmt) {
    std::shared_ptr<FuncCallExpr> p = std::static_pointer_cast<FuncCallExpr>(stmt);
    Function* func = module->getFunction(p->Id->Id);
    std::vector<Value*> args;
    for(auto it : p->Args->Parameters) {
        args.push_back(CodeGenForStmt(it));
    }
    CallInst *call = CallInst::Create(func, makeArrayRef(args), "", CurrentBlock);
    return call;
}

Value* LLVMVisitor::CodeGenForIfStmt(std::shared_ptr<Stmt> stmt) {
    std::shared_ptr<IfStmt> p = std::static_pointer_cast<IfStmt>(stmt);
    Value* CondV = CodeGenForStmt(p->Cond);
    if (isa<AllocaInst>(CondV)) {
        CondV = builder->CreateLoad(CondV);
    }
    Value *Zero = ConstantInt::get(getGlobalContext(), APInt(32, 0));
    CondV = builder->CreateICmpNE(CondV, Zero);
    Function *TheFunction = builder->GetInsertBlock()->getParent();
    BasicBlock *ThenBB = BasicBlock::Create(getGlobalContext(), "then", TheFunction);
    BasicBlock *ElseBB = BasicBlock::Create(getGlobalContext(), "else", TheFunction);
    builder->CreateCondBr(CondV, ThenBB, ElseBB);
    builder->SetInsertPoint(ThenBB);
    CodeGenForStmt(p->Then);
    ThenBB = builder->GetInsertBlock();
    builder->CreateBr(ElseBB);
    builder->SetInsertPoint(ElseBB);
    if (p->Else) {
        CodeGenForStmt(p->Else);
    }
    ElseBB = builder->GetInsertBlock();
    return NULL;
}

Value* LLVMVisitor::CodeGenForAssignStmt(std::shared_ptr<Stmt> stmt) {
    std::shared_ptr<AssignStmt> p = std::static_pointer_cast<AssignStmt>(stmt);
    Value* l = this->CodeGenForStmt(p->left);
    Value* r = this->CodeGenForStmt(p->right);
    if (NULL == CurrentFunction) {
        if (p->left->getSelfName() == "VarExpr") {
            static_cast<GlobalVariable*>(l)->setInitializer(static_cast<Constant*>(r));
            return NULL;
        }
    }
    l = GetLeftValue(l);
    r = GetRightValue(r);
    return this->builder->CreateStore(r,l);
}
