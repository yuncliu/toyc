#include "Visitor.h"
#include "ast.h"
#include "Expr.h"
#include "Stmt.h"

Visitor::Visitor():indent(0) {
    functions.insert(std::pair<std::string, TRAVEL_FUNC>("CompoundStmt", &Visitor::TravelCompoundStmt));
    functions.insert(std::pair<std::string, TRAVEL_FUNC>("Func", &Visitor::TravelFunc));
    functions.insert(std::pair<std::string, TRAVEL_FUNC>("FuncProtoType", &Visitor::TravelFuncProtoType));
    functions.insert(std::pair<std::string, TRAVEL_FUNC>("IdExpr", &Visitor::TravelIdExpr));
    functions.insert(std::pair<std::string, TRAVEL_FUNC>("TypeExpr", &Visitor::TravelTypeExpr));
    functions.insert(std::pair<std::string, TRAVEL_FUNC>("FuncParameter", &Visitor::TravelFuncParameter));
    functions.insert(std::pair<std::string, TRAVEL_FUNC>("VarExpr", &Visitor::TravelVarExpr));
    functions.insert(std::pair<std::string, TRAVEL_FUNC>("BinaryExpr", &Visitor::TravelBinaryExpr));
    functions.insert(std::pair<std::string, TRAVEL_FUNC>("ReturnStmt", &Visitor::TravelReturnStmt));
    functions.insert(std::pair<std::string, TRAVEL_FUNC>("IntExpr", &Visitor::TravelIntExpr));
}

Visitor::~Visitor() {
}

TRAVEL_FUNC Visitor::getFunction(std::string name) {
    std::map<std::string, TRAVEL_FUNC>::iterator it = functions.find(name);
    if (it == functions.end() ){
        return NULL;
    }
    return it->second;
}

bool Visitor::Travel(Stmt* s) {
    TRAVEL_FUNC travel_func = this->getFunction(s->getSelfName());
    if (NULL == travel_func) {
        printf("No visit function for [%s]\n", s->getSelfName().c_str());
        return false;
    }
    if (this->Visit(s)) {
        return (this->*travel_func)(s);
    }
    return true;
}

bool Visitor::TravelCompoundStmt(Stmt* stmt) {
    CompoundStmt* p = (CompoundStmt*)stmt;
    this->indent++;
    for (auto it: p->stmts) {
        this->Travel(it);
    }
    this->indent--;
    return true;
}

bool Visitor::TravelFunc(Stmt* stmt) {
    Func* f = (Func*)stmt;
    this->indent++;
    this->Travel(f->ProtoType);
    this->Travel(f->FuncBody);
    this->indent--;
    return true;
}

bool Visitor::TravelFuncProtoType(Stmt* stmt) {
    FuncProtoType* f = (FuncProtoType*)stmt;
    this->indent++;
    this->Travel(f->Id);
    this->Travel(f->ReturnTy);
    this->Travel(f->Param);
    this->indent--;
    return true;
}
bool Visitor::TravelIdExpr(Stmt* stmt) {
    return true;
}

bool Visitor::TravelTypeExpr(Stmt* stmt) {
    return true;
}

bool Visitor::TravelFuncParameter(Stmt* stmt) {
    FuncParameter* p = (FuncParameter*)stmt;
    this->indent++;
    for (auto it: p->Params) {
        this->Travel(it);
    }
    this->indent--;
    return true;
}

bool Visitor::TravelVarExpr(Stmt* stmt) {
    VarExpr* p = (VarExpr*)stmt;
    this->indent++;
    this->Travel(p->Type);
    this->Travel(p->Id);
    this->indent--;
    return true;
}

bool Visitor::TravelBinaryExpr(Stmt* stmt) {
    BinaryExpr* p = (BinaryExpr*)stmt;
    this->indent++;
    this->Travel(p->left);
    this->Travel(p->right);
    this->indent--;
    return true;
}

bool Visitor::TravelReturnStmt(Stmt* stmt) {
    ReturnStmt* p = (ReturnStmt*)stmt;
    this->indent++;
    this->Travel(p->Ret);
    this->indent--;
    return true;
}

bool Visitor::TravelIntExpr(Stmt* stmt) {
    return true;
}

bool Visitor::Visit(Stmt* stmt) {
    return true;
}
