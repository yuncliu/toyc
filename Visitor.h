#ifndef _VISITOR_H_
#define _VISITOR_H_

class Expr;
class Stmt;
class Func;
class FuncProtoType;
class CompoundStmt;
class VarExpr;

class Visitor {
    public:
        int indent;
        Visitor();
        virtual ~Visitor();
        void print_indent();
        void VisitExpr(Expr* expr);
        void VisitStmt(Stmt* stmt);
        void VisitCompundStmt(CompoundStmt* stmt);
        void VisitFunc(Func* func);
        void VisitFuncProtoType(FuncProtoType* proto);
        void VisitVarExpr(VarExpr* var);
};

#endif // _VISITOR_H_
