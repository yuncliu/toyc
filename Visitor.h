#ifndef _VISITOR_H_
#define _VISITOR_H_
#include <map>
class Expr;
class Stmt;
class Func;
class FuncProtoType;
class CompoundStmt;
class VarExpr;
class Visitor;
typedef void (Visitor::*VISIT_FUNC)(Stmt* s);

class Visitor {
    std::map<std::string, VISIT_FUNC> functions;
    public:
        int indent;
        Visitor();
        virtual ~Visitor();
        VISIT_FUNC getFunction(std::string name);
        void Visit(Stmt*);
        void print_indent();
        void VisitCompoundStmt(Stmt* stmt);
        void VisitFunc(Stmt* stmt);
        void VisitFuncProtoType(Stmt* stmt);
        void VisitIdExpr(Stmt* stmt);
        void VisitTypeExpr(Stmt* stmt);
        void VisitFuncParameter(Stmt* stmt);
        void VisitVarExpr(Stmt* stmt);
        void VisitBinaryExpr(Stmt* stmt);
        void VisitReturnStmt(Stmt* stmt);
        void VisitIntExpr(Stmt* stmt);
};

#endif // _VISITOR_H_
