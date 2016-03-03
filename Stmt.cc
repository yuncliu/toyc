#include "ast.h"
#include "Expr.h"
#include "Stmt.h"

//Stmt
Stmt::Stmt(std::string name): Name("Stmt") {
}

Stmt::~Stmt() {
}

void Stmt::Accept(Visitor* v) {
}

std::string Stmt::getSelfName() {
    return this->Name;
}

// ReturnStmt
ReturnStmt::ReturnStmt(Stmt* ret): Stmt("ReturnStmt"), Ret(ret){
}

ReturnStmt::~ReturnStmt() {
}

// CompoundStmt
CompoundStmt::CompoundStmt(): Stmt("CompoundStmt") {
}

CompoundStmt::~CompoundStmt() {
}

void CompoundStmt::addStatement(Stmt* s) {
    this->stmts.push_back(s);
}

void CompoundStmt::Accept(Visitor* v) {
    v->VisitCompundStmt(this);
}

// FuncParamter
FuncParameter::FuncParameter():Stmt("FuncParamter") {
}

FuncParameter::~FuncParameter() {
}

void FuncParameter::addParam(Stmt* v) {
    Params.push_back(v);
}

/**
 *Function
 *param[in] i      id of function name
 *param[in] rty    return type
 *param[in] args   args
 */
FuncProtoType::FuncProtoType(IdExpr* i, TypeExpr* rty, FuncParameter* param)
    :Stmt("FuncProtoType"), Id(i), ReturnTy(rty), Param(param) {
}

FuncProtoType::~FuncProtoType(){
}

// FuncCallArgs
FuncCallParams::FuncCallParams():Stmt("FuncCallParams") {
}

FuncCallParams::~FuncCallParams() {
}

void FuncCallParams::pushParam(Stmt* param) {
    this->Parameters.push_back(param);
}

// Func
Func::Func(FuncProtoType* f, CompoundStmt* b)
    :Stmt("Func"), ProtoType(f), FuncBody(b){
}

Func::~Func() {
}

IfStmt::IfStmt(Stmt* Cond, CompoundStmt* Then, CompoundStmt* Else)
    :Stmt("IfStmt"),Cond(Cond),Then(Then), Else(Else) {
    }
IfStmt::~IfStmt() {
}
