#ifndef _STMT_H_
#define _STMT_H_
#include <string>
#include <vector>
#include <memory>
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
        virtual std::string getSelfName();
};

class ReturnStmt: public Stmt {
    public:
        std::shared_ptr<Stmt> Ret;
    public:
        ReturnStmt(std::shared_ptr<Stmt> ret);
        ~ReturnStmt();
};

class CompoundStmt:public Stmt {
    public:
        //std::vector<Stmt*> stmts;
        std::vector<std::shared_ptr<Stmt> > stmts;
    public:
        CompoundStmt();
        ~CompoundStmt();
        void addStatement(std::shared_ptr<Stmt> s);
};

class FuncParameter: public Stmt {
    public:
        std::vector<std::shared_ptr<Stmt> >  Params;
    public:
        FuncParameter();
        ~FuncParameter();
        void addParam(std::shared_ptr<Stmt> v);
};

class FuncProtoType: public Stmt {
    public:
        std::shared_ptr<IdExpr> Id;
        std::shared_ptr<TypeExpr> ReturnTy;
        std::shared_ptr<FuncParameter> Param;
    public:
        FuncProtoType(std::shared_ptr<IdExpr> i,
                std::shared_ptr<TypeExpr> rty,
                std::shared_ptr<FuncParameter> param);
        ~FuncProtoType();
};

class FuncCallParams: public Stmt {
    public:
        std::vector<std::shared_ptr<Stmt> > Parameters;
    public:
        FuncCallParams();
        ~FuncCallParams();
        void pushParam(std::shared_ptr<Stmt> parm);
};

class Func:public Stmt {
    public:
        std::shared_ptr<FuncProtoType> ProtoType;
        std::shared_ptr<CompoundStmt> FuncBody;
    public:
        Func(std::shared_ptr<FuncProtoType> f,
                std::shared_ptr<CompoundStmt> b);
        ~Func();
};

class IfStmt:public Stmt {
    public:
        std::shared_ptr<Stmt> Cond;
        std::shared_ptr<Stmt> Then;
        std::shared_ptr<Stmt> Else;
    public:
        IfStmt(std::shared_ptr<Stmt> Cond,
                std::shared_ptr<Stmt>Then,
                std::shared_ptr<Stmt>Else);
        IfStmt(std::shared_ptr<Stmt> Cond,
                std::shared_ptr<Stmt>Then);
        ~IfStmt();
};

class AssignStmt:public Stmt {
    public:
        std::shared_ptr<Stmt> left;
        std::shared_ptr<Stmt> right;
    public:
        AssignStmt(std::shared_ptr<Stmt> left,
                std::shared_ptr<Stmt> right);
        ~AssignStmt();
};
#endif // _STMT_H_
