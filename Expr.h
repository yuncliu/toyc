#ifndef __EXPR_H_
#define __EXPR_H_
#include "Stmt.h"
class IdExpr: public Stmt{
    public:
        std::string Id;
    public:
        IdExpr(std::string id);
        virtual ~IdExpr();
};

class TypeExpr: public Stmt {
    public:
        std::string Type;
    public:
        TypeExpr(std::string type);
        ~TypeExpr();
};

class IntExpr: public Stmt {
    public:
        int value;
    public:
        IntExpr(int i);
        ~IntExpr();
};

class DoubleExpr: public Stmt {
    public:
        double value;
    public:
        DoubleExpr(double d);
        ~DoubleExpr();
};

class VarExpr: public Stmt {
    public:
        std::shared_ptr<TypeExpr> Type;
        std::shared_ptr<IdExpr> Id;
    public:
        VarExpr(std::shared_ptr<TypeExpr> ty, std::shared_ptr<IdExpr> id);
        ~VarExpr();
};

class FuncCallExpr: public Stmt {
    public:
        std::shared_ptr<IdExpr> Id;
        std::shared_ptr<FuncCallParams> Args;
    public:
        FuncCallExpr(std::shared_ptr<IdExpr> id, std::shared_ptr<FuncCallParams> args);
        ~FuncCallExpr();
};

class BinaryExpr:public Stmt {
    public:
        char op;
        std::shared_ptr<Stmt> left;
        std::shared_ptr<Stmt> right;
    public:
        BinaryExpr(char op, std::shared_ptr<Stmt> l, std::shared_ptr<Stmt> r);
        virtual ~BinaryExpr();
};
#endif
