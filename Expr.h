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
        TypeExpr* Type;
        IdExpr* Id;
    public:
        VarExpr(TypeExpr* ty, IdExpr* id);
        ~VarExpr();
};

class FuncCallExpr: public Stmt {
    public:
        IdExpr* Id;
        FuncCallParams* Args;
    public:
        FuncCallExpr(IdExpr* id, FuncCallParams* args);
        ~FuncCallExpr();
};

class BinaryExpr:public Stmt {
    public:
        char op;
        Stmt* left;
        Stmt* right;
    public:
        BinaryExpr(char op, Stmt* l, Stmt* r);
        virtual ~BinaryExpr();
};
#endif
