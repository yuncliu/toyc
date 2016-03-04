#include "DumpVisitor.h"
#include "ast.h"
#include "Expr.h"
#include "Stmt.h"

DumpVisitor::DumpVisitor():indent(0) {
    functions.insert(std::pair<std::string, VISIT_FUNC>("CompoundStmt", &DumpVisitor::VisitCompoundStmt));
    functions.insert(std::pair<std::string, VISIT_FUNC>("Func", &DumpVisitor::VisitFunc));
    functions.insert(std::pair<std::string, VISIT_FUNC>("FuncProtoType", &DumpVisitor::VisitFuncProtoType));
    functions.insert(std::pair<std::string, VISIT_FUNC>("IdExpr", &DumpVisitor::VisitIdExpr));
    functions.insert(std::pair<std::string, VISIT_FUNC>("TypeExpr", &DumpVisitor::VisitTypeExpr));
    functions.insert(std::pair<std::string, VISIT_FUNC>("FuncParameter", &DumpVisitor::VisitFuncParameter));
    functions.insert(std::pair<std::string, VISIT_FUNC>("VarExpr", &DumpVisitor::VisitVarExpr));
    functions.insert(std::pair<std::string, VISIT_FUNC>("BinaryExpr", &DumpVisitor::VisitBinaryExpr));
    functions.insert(std::pair<std::string, VISIT_FUNC>("ReturnStmt", &DumpVisitor::VisitReturnStmt));
    functions.insert(std::pair<std::string, VISIT_FUNC>("IntExpr", &DumpVisitor::VisitIntExpr));
}

DumpVisitor::~DumpVisitor() {
}

VISIT_FUNC DumpVisitor::getFunction(std::string name) {
    std::map<std::string, VISIT_FUNC>::iterator it = functions.find(name);
    if (it == functions.end() ){
        return NULL;
    }
    return it->second;
}

bool DumpVisitor::Visit(Stmt* s) {
    VISIT_FUNC visit_func = this->getFunction(s->getSelfName());
    if (NULL == visit_func) {
        printf("No visit function for [%s]\n", s->getSelfName().c_str());
        return false;
    }
    print_indent();
    return (this->*visit_func)(s);
}

void DumpVisitor::print_indent() {
    for (int i = 0; i < this->indent; ++i) {
        printf("  ");
    }
}

bool DumpVisitor::VisitCompoundStmt(Stmt* stmt) {
    CompoundStmt* p = (CompoundStmt*)stmt;
    printf("ConmpoundStmt:\n");
    this->indent++;
    for (auto it: p->stmts) {
        this->Visit(it);
    }
    this->indent--;
    return true;
}

bool DumpVisitor::VisitFunc(Stmt* stmt) {
    Func* f = (Func*)stmt;
    printf("Function: name [%s] return type [%s]\n",
            f->ProtoType->Id->Id.c_str(),
            f->ProtoType->ReturnTy->Type.c_str());
    this->indent++;
    this->Visit(f->ProtoType);
    this->Visit(f->FuncBody);
    this->indent--;
    return true;
}

bool DumpVisitor::VisitFuncProtoType(Stmt* stmt) {
    printf("FunctionProtoType\n");
    FuncProtoType* f = (FuncProtoType*)stmt;
    this->indent++;
    this->Visit(f->Id);
    this->Visit(f->ReturnTy);
    this->Visit(f->Param);
    this->indent--;
    return true;
}
bool DumpVisitor::VisitIdExpr(Stmt* stmt) {
    IdExpr* p = static_cast<IdExpr*>(stmt);
    printf("ID:[%s]\n", p->Id.c_str());
    return true;
}

bool DumpVisitor::VisitTypeExpr(Stmt* stmt) {
    TypeExpr* p = static_cast<TypeExpr*>(stmt);
    printf("TypeExpr: [%s]\n", p->Type.c_str());
    return true;
}

bool DumpVisitor::VisitFuncParameter(Stmt* stmt) {
    FuncParameter* p = (FuncParameter*)stmt;
    if (p->Params.size() == 0) {
        printf("No Parameters\n");
        return true;
    }
    printf("FuncParameter\n");
    this->indent++;
    for (auto it: p->Params) {
        this->Visit(it);
    }
    this->indent--;
    return true;
}

bool DumpVisitor::VisitVarExpr(Stmt* stmt) {
    VarExpr* p = (VarExpr*)stmt;
    printf("VarExpr: Type[%s] Id[%s]\n", p->Type->Type.c_str(), p->Id->Id.c_str());
    //this->indent++;
    //this->Visit(p->Type);
    //this->Visit(p->Id);
    //this->indent--;
    return true;
}

bool DumpVisitor::VisitBinaryExpr(Stmt* stmt) {
    BinaryExpr* p = (BinaryExpr*)stmt;
    printf("BinaryExpr: [%c]\n", p->op);
    this->indent++;
    this->Visit(p->left);
    this->Visit(p->right);
    this->indent--;
    return true;
}

bool DumpVisitor::VisitReturnStmt(Stmt* stmt) {
    ReturnStmt* p = static_cast<ReturnStmt*>(stmt);
    printf("ReturnStmt:\n");
    this->indent++;
    this->Visit(p->Ret);
    this->indent--;
    return true;
}

bool DumpVisitor::VisitIntExpr(Stmt* stmt) {
    IntExpr* p = static_cast<IntExpr*>(stmt);
    printf("IntExpr Value = [%d]\n", p->value);
    return true;
}
