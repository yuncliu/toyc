#ifndef _STMT_H_
#define _STMT_H_
#include <string>
class CompoundStmt;
class Visitor;
class IdExpr;
class TypeExpr;

class Stmt {
    public:
        std::string Name;
    public:
        Stmt(std::string name);
        ~Stmt();
        virtual void Accept(Visitor* v);
        virtual std::string getSelfName();
};

class ReturnStmt: public Stmt {
    public:
        Stmt* Ret;
    public:
        ReturnStmt(Stmt* ret);
        ~ReturnStmt();
};

class CompoundStmt:public Stmt {
    public:
        std::vector<Stmt*> stmts;
    public:
        CompoundStmt();
        ~CompoundStmt();
        void addStatement(Stmt* s);
};

class FuncParameter: public Stmt {
    public:
        std::vector<Stmt*>  Params;
    public:
        FuncParameter();
        ~FuncParameter();
        void addParam(Stmt* v);
};

class FuncProtoType: public Stmt {
    public:
        IdExpr* Id;
        TypeExpr* ReturnTy;
        FuncParameter* Param;
    public:
        FuncProtoType(IdExpr* i, TypeExpr* rty, FuncParameter* param);
        ~FuncProtoType();
};

class FuncCallParams: public Stmt {
    public:
        std::vector<Stmt*> Parameters;
    public:
        FuncCallParams();
        ~FuncCallParams();
        void pushParam(Stmt* parm);
};

class Func:public Stmt {
    public:
        FuncProtoType* ProtoType;
        CompoundStmt* FuncBody;
    public:
        Func(FuncProtoType* f, CompoundStmt* b);
        ~Func();
};

class IfStmt:public Stmt {
    public:
        Stmt* Cond;
        CompoundStmt* Then;
        CompoundStmt* Else;
    public:
        IfStmt(Stmt* Cond, CompoundStmt* Then, CompoundStmt* Else);
        ~IfStmt();
};
#endif // _STMT_H_
