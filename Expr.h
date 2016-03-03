#ifndef __EXPR_H_
#define __EXPR_H_
#include "Stmt.h"
class IdExpr: public Stmt{
    friend Visitor;
    std::string Id;
    public:
    IdExpr(std::string id);
    virtual ~IdExpr();
};

class TypeExpr: public Stmt {
    friend Visitor;
    std::string Type;
    public:
    TypeExpr(std::string type);
    ~TypeExpr();
};

class IntExpr: public Stmt {
    friend Visitor;
    int value;
    public:
    IntExpr(int i);
    ~IntExpr();
};

class DoubleExpr: public Stmt {
    friend Visitor;
    double value;
    public:
    DoubleExpr(double d);
    ~DoubleExpr();
};

class VarExpr: public Stmt {
    friend Visitor;
    TypeExpr* Type;
    IdExpr* Id;
    public:
    VarExpr(TypeExpr* ty, IdExpr* id);
    ~VarExpr();
};

class FuncCallExpr: public Stmt {
    friend Visitor;
    IdExpr* Id;
    FuncCallParams* Args;
    public:
    FuncCallExpr(IdExpr* id, FuncCallParams* args);
    ~FuncCallExpr();
};

class BinaryExpr:public Stmt {
    char op;
    Stmt* left;
    Stmt* right;
    public:
    BinaryExpr(char op, Stmt* l, Stmt* r);
    virtual ~BinaryExpr();
};

#endif
