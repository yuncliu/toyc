#include "Expr.h"
#include "Stmt.h"

// IdExpr
IdExpr::IdExpr(std::string id): Stmt("IdExpr"), Id(id){
}

IdExpr::~IdExpr() {
}

TypeExpr::TypeExpr(std::string type):Stmt("TypeExpr"), Type(type) {
}

TypeExpr::~TypeExpr() {
}

// IntExpr
IntExpr::IntExpr(int i): Stmt("IntExpr"), value(i) {
}

IntExpr::~IntExpr() {
}

DoubleExpr::DoubleExpr(double d)
    :Stmt("DoubleExpr"), value(d) {
}

DoubleExpr::~DoubleExpr() {
}

//VarExpr
VarExpr::VarExpr(std::shared_ptr<TypeExpr> ty, std::shared_ptr<IdExpr> id)
    :Stmt("VarExpr"), Type(ty), Id(id) {
    }

VarExpr::~VarExpr() {
}

FuncCallExpr::FuncCallExpr(std::shared_ptr<IdExpr> id, std::shared_ptr<FuncCallParams> args)
    :Stmt("FuncCallExpr"), Id(id), Args(args) {
    }

FuncCallExpr::~FuncCallExpr() {
}

// BinaryExpr
BinaryExpr::BinaryExpr(char op, std::shared_ptr<Stmt> l, std::shared_ptr<Stmt> r)
    :Stmt("BinaryExpr"),op(op), left(l), right(r) {
    }

BinaryExpr::~BinaryExpr() {
}
