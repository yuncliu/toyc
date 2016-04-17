#include "Expr.h"
#include "Stmt.h"
#include "Visitor.h"

//Stmt
Stmt::Stmt(std::string name) {
    if (name.size() == 0 ) {
        Name = std::string("Stmt");
    }else {
        Name = name;
    }
}

Stmt::~Stmt() {
}

std::string Stmt::getSelfName() {
    return this->Name;
}

// ReturnStmt
ReturnStmt::ReturnStmt(std::shared_ptr<Stmt> ret): Stmt("ReturnStmt"), Ret(ret){
}

ReturnStmt::~ReturnStmt() {
}

// CompoundStmt
CompoundStmt::CompoundStmt(): Stmt("CompoundStmt") {
}

CompoundStmt::~CompoundStmt() {
}

void CompoundStmt::addStatement(std::shared_ptr<Stmt> s) {
    this->stmts.push_back(s);
}

// FuncParamter
FuncParameter::FuncParameter():Stmt("FuncParameter") {
}

FuncParameter::~FuncParameter() {
}

void FuncParameter::addParam(std::shared_ptr<Stmt> v){
    Params.push_back(v);
}

/**
 *Function
 *param[in] i      id of function name
 *param[in] rty    return type
 *param[in] args   args
 */
 FuncProtoType::FuncProtoType(std::shared_ptr<IdExpr> i,
                std::shared_ptr<TypeExpr> rty,
                std::shared_ptr<FuncParameter> param)
    :Stmt("FuncProtoType"), Id(i), ReturnTy(rty), Param(param) {
}

FuncProtoType::~FuncProtoType(){
}

// FuncCallArgs
FuncCallParams::FuncCallParams():Stmt("FuncCallParams") {
}

FuncCallParams::~FuncCallParams() {
}

void FuncCallParams::pushParam(std::shared_ptr<Stmt> param) {
    this->Parameters.push_back(param);
}

// Func
Func::Func(std::shared_ptr<FuncProtoType> f,
        std::shared_ptr<CompoundStmt> b)
    :Stmt("Func"), ProtoType(f), FuncBody(b){
}

Func::~Func() {
}

IfStmt::IfStmt(std::shared_ptr<Stmt> Cond,
                std::shared_ptr<Stmt>Then,
                std::shared_ptr<Stmt>Else)
:Stmt("IfStmt"),Cond(Cond),Then(Then), Else(Else) {
}
IfStmt::IfStmt(std::shared_ptr<Stmt> Cond,
                std::shared_ptr<Stmt>Then)
:Stmt("IfStmt"),Cond(Cond),Then(Then){
}

IfStmt::~IfStmt() {
}

// AssignStmt
AssignStmt::AssignStmt(std::shared_ptr<Stmt> left,
    std::shared_ptr<Stmt> right)
:Stmt("AssignStmt"), left(left), right(right) {
}

AssignStmt::~AssignStmt() {
}
