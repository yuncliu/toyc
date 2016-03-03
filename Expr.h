#ifndef __EXPR_H_
#define __EXPR_H_
class BlockAST;
class FuncCallArgs;
class Expr {
    public:
        Expr();
        virtual ~Expr();
        virtual Value* codegen(BlockAST* block);
};

class IdExpr:public Expr{
        std::string      name;
    public:
        IdExpr(std::string s);
        virtual ~IdExpr();
        Value* codegen(BlockAST* block);
        std::string getName();
};

class IntExpr: Expr {
        int value;
    public:
        IntExpr(int i);
        ~IntExpr();
        Value* codegen(BlockAST* block);
};

class DoubleExpr: Expr {
        double value;
    public:
        DoubleExpr(double d);
        ~DoubleExpr();
        Value* codegen(BlockAST* block);
};

class VarExpr: Expr {
        Type* type;
        IdExpr* Id;
    public:
        VarExpr(Type* ty, IdExpr* id);
        ~VarExpr();
        Value* codegen(BlockAST* block);
        Type* getType();
        std::string getName();
        std::string Info();
};

class FuncCallExpr: Expr {
    IdExpr* Id;
    FuncCallArgs* Args;
    public:
        FuncCallExpr(IdExpr* id, FuncCallArgs* args);
        ~FuncCallExpr();
        Value* codegen(BlockAST* block);
};

class BinaryExpr {
        char op;
        Expr* left;
        Expr* right;
    public:
        BinaryExpr(char op, Expr* l, Expr* r);
        virtual ~BinaryExpr();
        virtual Value* codegen(BlockAST* block);
};

#endif
