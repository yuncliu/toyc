#include "LLVMVisitor.h"
#include "ast.h"
#include "Expr.h"
#include "Stmt.h"

LLVMVisitor::LLVMVisitor() {
    functions.insert(std::pair<std::string, VISIT_FUNC>("CompoundStmt", &LLVMVisitor::VisitCompoundStmt));
    functions.insert(std::pair<std::string, VISIT_FUNC>("Func", &LLVMVisitor::VisitFunc));
    functions.insert(std::pair<std::string, VISIT_FUNC>("FuncProtoType", &LLVMVisitor::VisitFuncProtoType));
    functions.insert(std::pair<std::string, VISIT_FUNC>("IdExpr", &LLVMVisitor::VisitIdExpr));
    functions.insert(std::pair<std::string, VISIT_FUNC>("TypeExpr", &LLVMVisitor::VisitTypeExpr));
    functions.insert(std::pair<std::string, VISIT_FUNC>("FuncParameter", &LLVMVisitor::VisitFuncParameter));
    functions.insert(std::pair<std::string, VISIT_FUNC>("VarExpr", &LLVMVisitor::VisitVarExpr));
    functions.insert(std::pair<std::string, VISIT_FUNC>("BinaryExpr", &LLVMVisitor::VisitBinaryExpr));
    functions.insert(std::pair<std::string, VISIT_FUNC>("ReturnStmt", &LLVMVisitor::VisitReturnStmt));
    functions.insert(std::pair<std::string, VISIT_FUNC>("IntExpr", &LLVMVisitor::VisitIntExpr));
    module = new Module("main", getGlobalContext());
    builder = new IRBuilder<>(getGlobalContext());
}

LLVMVisitor::~LLVMVisitor() {
    this->module->dump();
}

LLVMVisitor::VISIT_FUNC LLVMVisitor::getFunction(std::string name) {
    std::map<std::string, VISIT_FUNC>::iterator it = functions.find(name);
    if (it == functions.end() ){
        return NULL;
    }
    return it->second;
}

bool LLVMVisitor::Visit(Stmt* s) {
    VISIT_FUNC visit_func = this->getFunction(s->getSelfName());
    if (NULL == visit_func) {
        printf("No visit function for [%s]\n", s->getSelfName().c_str());
        return false;
    }
    return (this->*visit_func)(s);
}

bool LLVMVisitor::VisitCompoundStmt(Stmt* stmt) {
    CompoundStmt* p = (CompoundStmt*)stmt;
    int size = p->stmts.size();
    for (int i = 0; i < size; ++i) {
        this->Visit(p->stmts[i]);
    }
    return true;
}

bool LLVMVisitor::VisitFunc(Stmt* stmt) {
    Func* f = (Func*)stmt;
    Function* func = CodeGenForFuncProtoType(f->ProtoType);
    BasicBlock *block = BasicBlock::Create(getGlobalContext(), "entry", func);
    builder->SetInsertPoint(block);
    //this->Visit(f->FuncBody);
    this->CodeGenForCompoundStmt(f->FuncBody);
    return true;
}

bool LLVMVisitor::VisitFuncProtoType(Stmt* stmt) {
    FuncProtoType* f = (FuncProtoType*)stmt;
    this->Visit(f->Id);
    this->Visit(f->ReturnTy);
    this->Visit(f->Param);
    return true;
}

bool LLVMVisitor::VisitIdExpr(Stmt* stmt) {
    IdExpr* p = static_cast<IdExpr*>(stmt);
    printf("ID:[%s]\n", p->Id.c_str());
    return true;
}

bool LLVMVisitor::VisitTypeExpr(Stmt* stmt) {
    TypeExpr* p = static_cast<TypeExpr*>(stmt);
    printf("TypeExpr: [%s]\n", p->Type.c_str());
    return true;
}

bool LLVMVisitor::VisitFuncParameter(Stmt* stmt) {
    FuncParameter* p = (FuncParameter*)stmt;
    if (p->Params.size() == 0) {
        printf("No Parameters\n");
        return true;
    }
    printf("FuncParameter\n");
    int size = p->Params.size();
    for (int i = 0; i < size; ++i) {
        this->Visit(p->Params[i]);
    }
    return true;
}

bool LLVMVisitor::VisitVarExpr(Stmt* stmt) {
    VarExpr* p = (VarExpr*)stmt;
    //printf("VarExpr: Type[%s] Id[%s]\n", p->Type->Type.c_str(), p->Id->Id.c_str());
    this->Visit(p->Type);
    this->Visit(p->Id);
    GlobalVariable* v = new GlobalVariable(*module, Type::getInt32Ty(getGlobalContext()),
            false, Function::InternalLinkage, NULL);
    v->setName(p->Id->Id.c_str());
    GlobalNamedValue.insert(std::pair<std::string, Value*>(p->Id->Id, v));
    return true;
}

bool LLVMVisitor::VisitBinaryExpr(Stmt* stmt) {
    BinaryExpr* p = (BinaryExpr*)stmt;
    printf("BinaryExpr: [%c]\n", p->op);
    this->Visit(p->left);
    this->Visit(p->right);
    return true;
}

bool LLVMVisitor::VisitReturnStmt(Stmt* stmt) {
    ReturnStmt* p = static_cast<ReturnStmt*>(stmt);
    printf("ReturnStmt:\n");
    this->Visit(p->Ret);
    return true;
}

bool LLVMVisitor::VisitIntExpr(Stmt* stmt) {
    IntExpr* p = static_cast<IntExpr*>(stmt);
    printf("IntExpr Value = [%d]\n", p->value);
    return true;
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
    //FuncParameter* p = (FuncParameter*)stmt;
    std::vector<Type*> v;
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
    printf("Code gen for [%s]\n", stmt->getSelfName().c_str());
    return NULL;
}
