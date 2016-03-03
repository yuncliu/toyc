#ifndef _STMT_H_
#define _STMT_H_
#include <string>
class CompoundStmt;
class Visitor;
class IdExpr;
class TypeExpr;

class Stmt {
        friend Visitor;
    protected:
        std::string Name;
    public:
        Stmt(std::string name);
        ~Stmt();
        virtual void Accept(Visitor* v);
        virtual std::string getSelfName();
};

class ReturnStmt: public Stmt {
    friend Visitor;
    Stmt* Ret;
    public:
        ReturnStmt(Stmt* ret);
        ~ReturnStmt();
};

class CompoundStmt:public Stmt {
        friend Visitor;
        std::vector<Stmt*> stmts;
    public:
        CompoundStmt();
        ~CompoundStmt();
        void addStatement(Stmt* s);
};

class FuncParameter: public Stmt {
        friend Visitor;
        std::vector<Stmt*>  Params;
    public:
        FuncParameter();
        ~FuncParameter();
        void addParam(Stmt* v);
};

class FuncProtoType: public Stmt {
        friend Visitor;
        IdExpr* Id;
        TypeExpr* ReturnTy;
        FuncParameter* Param;
    public:
        FuncProtoType(IdExpr* i, TypeExpr* rty, FuncParameter* param);
        ~FuncProtoType();
};

class FuncCallParams: public Stmt {
    friend Visitor;
    std::vector<Stmt*> Parameters;
    public:
    FuncCallParams();
    ~FuncCallParams();
    void pushParam(Stmt* parm);
};

class Func:public Stmt {
        friend Visitor;
        FuncProtoType* ProtoType;
        CompoundStmt* FuncBody;
    public:
        Func(FuncProtoType* f, CompoundStmt* b);
        ~Func();
};

class IfStmt:public Stmt {
    Stmt* Cond;
    CompoundStmt* Then;
    CompoundStmt* Else;
    public:
    IfStmt(Stmt* Cond, CompoundStmt* Then, CompoundStmt* Else);
    ~IfStmt();
};
#endif // _STMT_H_
