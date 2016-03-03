#include "Visitor.h"
#include "ast.h"
#include "Expr.h"
#include "Stmt.h"

Visitor::Visitor():indent(0) {
    functions.insert(std::pair<std::string, VISIT_FUNC>("CompoundStmt", &Visitor::VisitCompoundStmt));
    functions.insert(std::pair<std::string, VISIT_FUNC>("Func", &Visitor::VisitFunc));
    functions.insert(std::pair<std::string, VISIT_FUNC>("FuncProtoType", &Visitor::VisitFuncProtoType));
    functions.insert(std::pair<std::string, VISIT_FUNC>("IdExpr", &Visitor::VisitIdExpr));
    functions.insert(std::pair<std::string, VISIT_FUNC>("TypeExpr", &Visitor::VisitTypeExpr));
    functions.insert(std::pair<std::string, VISIT_FUNC>("FuncParameter", &Visitor::VisitFuncParameter));
    functions.insert(std::pair<std::string, VISIT_FUNC>("VarExpr", &Visitor::VisitVarExpr));
    functions.insert(std::pair<std::string, VISIT_FUNC>("BinaryExpr", &Visitor::VisitBinaryExpr));
    functions.insert(std::pair<std::string, VISIT_FUNC>("ReturnStmt", &Visitor::VisitReturnStmt));
    functions.insert(std::pair<std::string, VISIT_FUNC>("IntExpr", &Visitor::VisitIntExpr));
}

Visitor::~Visitor() {
}

VISIT_FUNC Visitor::getFunction(std::string name) {
    std::map<std::string, VISIT_FUNC>::iterator it = functions.find(name);
    if (it == functions.end() ){
        return NULL;
    }
    return it->second;
}
void Visitor::Visit(Stmt* s) {
    VISIT_FUNC func = this->getFunction(s->getSelfName());
    if (NULL == func) {
        printf("No visit function for [%s]\n", s->getSelfName().c_str());
        return;
    }
    (this->*func)(s);
}

void Visitor::print_indent() {
    for (int i = 0; i < this->indent; ++i) {
        printf("  ");
    }
}

void Visitor::VisitCompoundStmt(Stmt* stmt) {
    CompoundStmt* p = (CompoundStmt*)stmt;
    print_indent();
    printf("%s\n", "CompoundStmt");
    this->indent++;
    for (auto it: p->stmts) {
        this->Visit(it);
    }
    this->indent--;
}

void Visitor::VisitFunc(Stmt* stmt) {
    print_indent();
    Func* f = (Func*)stmt;
    printf("%s\n", "Function");
    this->indent++;
    this->Visit(f->ProtoType);
    this->Visit(f->FuncBody);
    this->indent--;
}
void Visitor::VisitFuncProtoType(Stmt* stmt) {
    print_indent();
    FuncProtoType* f = (FuncProtoType*)stmt;
    printf("%s\n", "FunctionProtoType");
    this->indent++;
    this->Visit(f->Id);
    this->Visit(f->ReturnTy);
    this->Visit(f->Param);
    this->indent--;
}
void Visitor::VisitIdExpr(Stmt* stmt) {
    print_indent();
    IdExpr* p = (IdExpr*)stmt;
    printf("IdExpr[%s]\n", p->Id.c_str());
}

void Visitor::VisitTypeExpr(Stmt* stmt) {
    print_indent();
    TypeExpr* p = (TypeExpr*)stmt;
    printf("[%d]TypeExpr[%s]\n", this->indent, p->Type.c_str());
}

void Visitor::VisitFuncParameter(Stmt* stmt) {
    print_indent();
    FuncParameter* p = (FuncParameter*)stmt;
    printf("FuncParameters\n");
    this->indent++;
    for (auto it: p->Params) {
        this->Visit(it);
    }
    this->indent--;
}
void Visitor::VisitVarExpr(Stmt* stmt) {
    print_indent();
    VarExpr* p = (VarExpr*)stmt;
    printf("Variable\n");
    this->indent++;
    this->Visit(p->Type);
    this->Visit(p->Id);
    this->indent--;
}

void Visitor::VisitBinaryExpr(Stmt* stmt) {
    print_indent();
    BinaryExpr* p = (BinaryExpr*)stmt;
    this->indent++;
    printf("BinaryExpr: operator[%c]\n", p->op);
    this->Visit(p->left);
    this->Visit(p->right);
    this->indent--;
}
void Visitor::VisitReturnStmt(Stmt* stmt) {
    print_indent();
    ReturnStmt* p = (ReturnStmt*)stmt;
    this->indent++;
    printf("ReturnStmt\n");
    this->Visit(p->Ret);
    this->indent--;
}

void Visitor::VisitIntExpr(Stmt* stmt) {
    print_indent();
    IntExpr* p = (IntExpr*)stmt;
    printf("IntExpr: [%d]\n", p->value);
}
